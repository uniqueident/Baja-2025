#include "Camera.hpp"
#include "Modules/GUI/Renderer/CameraUtils.hpp"
#include "Modules/GUI/Renderer/Renderer.hpp"

// std
#include <EGL/eglplatform.h>
#include <cstddef>
#include <epoxy/egl_generated.h>
#include <epoxy/gl_generated.h>
#include <libcamera/framebuffer.h>
#include <libcamera/request.h>
#include <memory>
#include <queue>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

// Pretty sure this is linux specific.
#include <libdrm/drm_fourcc.h>
#include <linux/dma-buf.h>

// libs
#include <epoxy/egl.h>

#include <libcamera/framebuffer_allocator.h>
#include <libcamera/color_space.h>
#include <libcamera/base/span.h>
#include <libcamera/formats.h>
#include <libcamera/stream.h>

namespace BB {

    namespace GL {

        #define CAMERA_VIEW_WIDTH 1280
        #define CAMERA_VIEW_HEIGHT 720

        Camera::Camera() { }

        Camera::Camera(std::shared_ptr<libcamera::Camera> camera) {
            p_Camera = camera;
            p_Camera->acquire();

            std::unique_ptr<libcamera::CameraConfiguration> config = 
                p_Camera->generateConfiguration({ libcamera::StreamRole::Viewfinder });

            libcamera::StreamConfiguration &streamConfig = config->at(0);
            std::cout << "Default viewfinder config is: " << streamConfig.toString() << std::endl;

            streamConfig.size.width = CAMERA_VIEW_WIDTH;
            streamConfig.size.height = CAMERA_VIEW_HEIGHT;
            streamConfig.pixelFormat = libcamera::formats::RGB888;

            config->validate();
            std::cout << "Validated viewfinder config is: " << streamConfig.toString() << std::endl;

            p_Camera->configure(config.get());

            // need to free stream at end
            this->p_Allocator = new libcamera::FrameBufferAllocator(p_Camera);

            for (libcamera::StreamConfiguration& cfg : *config) {
                int ret = this->p_Allocator->allocate(cfg.stream());

                if (ret < 0)
                    std::cerr << "Can't allocate buffers!" << std::endl;

                size_t allocated = this->p_Allocator->buffers(cfg.stream()).size();
                std::cout << "Allocated " << allocated << " buffers for stream." << std::endl;
            }
            
            this->p_Stream = streamConfig.stream();
            const std::vector<std::unique_ptr<libcamera::FrameBuffer>>& buffers = this->p_Allocator->buffers(this->p_Stream);

            for (unsigned int i = 0; i < buffers.size(); i++) {
                std::unique_ptr<libcamera::Request> request = this->p_Camera->createRequest();

                if (!request)
                    std::cerr << "Can't create request!" << std::endl;

                const std::unique_ptr<libcamera::FrameBuffer>& buffer = buffers[i];
                int ret = request->addBuffer(this->p_Stream, buffer.get());

                if (ret < 0)
                    std::cerr << "Can't set buffer for request!" << std::endl;

                m_Requests.push_back(std::move(request));
            }

            MakeRequests();
        }

        Camera::~Camera() {
            
        }

        void Camera::Start() {
            this->p_Camera->start();
        }

        void Camera::Stop() {
            p_Camera->stop();
        }

        const CamBuffer& Camera::GetFrame() {
            
        }

        void Camera::Clear() {
            this->Stop();

            m_Requests.clear();

            this->p_Camera->release();
            this->p_Camera.reset();
        }

        static CamStreamInfo GetStreamInfo(const libcamera::Stream* stream) {
            const libcamera::StreamConfiguration& cfg = stream->configuration();

            CamStreamInfo info;
            info.width = cfg.size.width;
            info.height = cfg.size.height;
            info.stride = cfg.stride;
            info.pixelFormat = cfg.pixelFormat;
            info.colorSpace = cfg.colorSpace;
            return info;
        }

        void Camera::RequestComplete(libcamera::Request* request) {
            if (request->status() == libcamera::Request::RequestCancelled)
                return;

            struct dma_buf_sync dma_sync { };
            dma_sync.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_READ;

            for (const auto& bufferMap : request->buffers()) {
                auto it = this->m_MappedBuffers.find(bufferMap.second);
                if (it == m_MappedBuffers.end())
                    std::cerr << "Failed to identify request complete buffer!" << std::endl;

                int ret = ::ioctl(bufferMap.second->planes()[0].fd.get(), DMA_BUF_IOCTL_SYNC, &dma_sync);
                if (ret)
                    std::cerr << "Failed to syncronize dma buf on request complete!" << std::endl;
            }

            CompletedRequest* r = new CompletedRequest(this->m_Sequence++, request);
            CompletedRequestPtr payload(r, [this](CompletedRequest* cr) { this->QueueRequest(cr); });
            {
                this->m_CompletedRequests.insert(r);
            }
        }

        void Camera::MakeRequests() {
            std::map<libcamera::Stream*, std::queue<libcamera::FrameBuffer*>> freeBuffers;

            for (auto& kv : this->m_Framebuffers) {
                freeBuffers[kv.first] = { };

                for (auto& b : kv.second)
                    freeBuffers[kv.first].push(b.get());
            }

            // TODO: Change to range or condition limited loop
            while (true) {
                for (libcamera::StreamConfiguration& config : *this->p_Configuration) {
                    libcamera::Stream* stream = config.stream();

                    if (stream == this->p_Configuration->at(0).stream()) {
                        if (freeBuffers[stream].empty()) {
                            return;
                        }

                        std::unique_ptr<Request> request = this->CreateRequest();
                        if (!request)
                            std::cerr << "Failed to make request!" << std::endl;

                        this->m_Requests.push_back(std::move(request));
                    }
                    else if (freeBuffers[stream].empty())
                        std::cerr << "Concurrent streams need matching numbers of buffers!" << std::endl;

                    libcamera::FrameBuffer* buffer = freeBuffers[stream].front();
                    freeBuffers[stream].pop();

                    if (this->m_Requests.back()->AddBuffer(stream, buffer) < 0)
                        std::cerr << "Failed to add buffer to request!" << std::endl;
                }
            }
        }

        std::unique_ptr<Request> Camera::CreateRequest(uint64_t cookie) {

        }

        void Camera::QueueRequest(CompletedRequest* cr) {
            BufferMap buffers(std::move(cr->buffers));

            bool requestFound;
            {
                auto it = this->m_CompletedRequests.find(cr);

                if (it != this->m_CompletedRequests.end()) {
                    requestFound = true;
                    this->m_CompletedRequests.erase(it);
                }
                else {
                    requestFound = false;
                }
            }

            libcamera::Request* request = cr->request;
            
        }

    }   // GL

}   // BB
