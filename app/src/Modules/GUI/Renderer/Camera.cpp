#include "Camera.hpp"

#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <libcamera/stream.h>
#include <memory>
#include <unistd.h>
#include <sys/mman.h>

// libs
#include <libcamera/framebuffer_allocator.h>
#include <libcamera/base/span.h>
#include <libcamera/formats.h>

namespace BB {

    namespace GL {

        #define CAMERA_VIEW_WIDTH 1280
        #define CAMERA_VIEW_HEIGHT 720

        Camera::Camera() : p_Stream(nullptr), p_Allocator(nullptr), m_Active(false) { }

        Camera::Camera(std::shared_ptr<libcamera::Camera> camera) : p_Stream(nullptr), p_Allocator(nullptr), m_Active(true) {
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

            this->p_Camera->requestCompleted.connect(RequestComplete);

            m_Texture = std::make_shared<Texture2D>();
            this->m_Texture->Generate(CAMERA_VIEW_WIDTH, CAMERA_VIEW_HEIGHT, nullptr);
        }

        Camera::~Camera() {
            
        }

        void Camera::Start() {
            this->p_Camera->start();

            for (std::unique_ptr<libcamera::Request>& request : m_Requests)
                this->p_Camera->queueRequest(request.get());
        }

        void Camera::Stop() {
            p_Camera->stop();
        }

        const CamBuffer& Camera::GetFrame() {
            // CamBuffer& buffer;
        }

        void Camera::Clear() {
            if (this->m_Active)
                this->Stop();

            this->p_Allocator->free(this->p_Stream);
            delete this->p_Allocator;

            m_Requests.clear();

            this->p_Camera->release();
            this->p_Camera.reset();
        }

        void Camera::RequestComplete(libcamera::Request* request) {
            if (request->status() == libcamera::Request::RequestCancelled)
                return;

            // Update The Texture Here

            const std::map<const libcamera::Stream*, libcamera::FrameBuffer*>& buffers = request->buffers();

            for (auto bufferPair : buffers) {
                libcamera::FrameBuffer* buffer = bufferPair.second;
                const libcamera::FrameMetadata& metadata = buffer->metadata();

                std::cout << "Frame " << metadata.sequence << ": bytes (" << metadata.planes()[0].bytesused << ")" << std::endl;

                size_t length = 0;
                for (const auto& plane : buffer->planes())
                    length += plane.length;

                m_Texture->Bind();

                auto* addr = static_cast<unsigned char*>(mmap(nullptr, buffer->planes()[0].length, PROT_READ, MAP_PRIVATE, buffer->planes()[0].fd.get(), 0));

                if (addr == MAP_FAILED)
                    std::cerr << "Failed to map framebuffer planes!" << std::endl;

                std::cout << *addr << std::endl;

                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CAMERA_VIEW_WIDTH, CAMERA_VIEW_HEIGHT, GL_R8, GL_UNSIGNED_BYTE, addr);
                glFinish();

                glBindTexture(GL_TEXTURE_2D, 0);
            }

            request->reuse(libcamera::Request::ReuseBuffers);
            p_Camera->queueRequest(request);
        }

    }   // GL

}   // BB
