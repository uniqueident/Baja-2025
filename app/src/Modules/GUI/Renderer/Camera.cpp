#include "Camera.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <freetype/config/ftoption.h>
#include <iomanip>
#include <memory>

// libs
#include <libcamera/framebuffer_allocator.h>

namespace BB {

    namespace GL {

        #define CAMERA_VIEW_WIDTH 640
        #define CAMERA_VIEW_HEIGHT 480

        Camera::Camera() : m_Texture(), p_Stream(nullptr), p_Allocator(nullptr), m_Active(false) { }

        Camera::Camera(std::shared_ptr<libcamera::Camera> camera) : m_Texture(std::make_shared<Texture2D>()), p_Stream(nullptr), p_Allocator(nullptr), m_Active(true) {
            p_Camera = camera;
            p_Camera->acquire();

            std::unique_ptr<libcamera::CameraConfiguration> config = 
                p_Camera->generateConfiguration({ libcamera::StreamRole::Viewfinder });

            libcamera::StreamConfiguration &streamConfig = config->at(0);
            std::cout << "Default viewfinder config is: " << streamConfig.toString() << std::endl;

            streamConfig.size.width = CAMERA_VIEW_WIDTH;
            streamConfig.size.height = CAMERA_VIEW_HEIGHT;
            // streamConfig.pixelFormat.fromString("RGB888");

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

        const FrameData Camera::GetFrame() {
            return { CAMERA_VIEW_WIDTH, CAMERA_VIEW_HEIGHT, this->m_Texture };
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

                std::cout << " seq: " << std::setw(6) << std::setfill('0') << metadata.sequence << " bytesused: ";

                unsigned int nplane = 0;
                for (const libcamera::FrameMetadata::Plane &plane : metadata.planes())
                {
                    std::cout << plane.bytesused;
                    if (++nplane < metadata.planes().size()) std::cout << "/";
                }

                std::cout << std::endl;

            }

            request->reuse(libcamera::Request::ReuseBuffers);
            p_Camera->queueRequest(request);
        }

    }   // GL

}   // BB
