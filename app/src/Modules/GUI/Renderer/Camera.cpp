#include "Camera.hpp"

// std
#include <iomanip>
#include <memory>
#include <vector>

// libs
#include <libcamera/camera.h>
#include <libcamera/control_ids.h>

namespace CB {

    namespace GL {

        static void ProcessRequest(libcamera::Request* request) {
            const libcamera::Request::BufferMap& buffers = request->buffers();

            for (auto bufferPair : buffers) {
                libcamera::FrameBuffer* buffer = bufferPair.second;
                const libcamera::FrameMetadata& metadata = buffer->metadata();

                std::cout << " seq: " << std::setw(6) << std::setfill('0') << metadata.sequence << " bytes used: ";

                unsigned int nplane = 0;
                for (const libcamera::FrameMetadata::Plane& plane : metadata.planes()) {
                    std::cout << plane.bytesused;

                    if (++nplane < metadata.planes().size())
                        std::cout << "/";
                }

                std::cout << std::endl;
            }

            request->reuse(libcamera::Request::ReuseBuffers);
            
        }

        static void RequestComplete(libcamera::Request* request) {
            if (request->status() == libcamera::Request::RequestCancelled)
                return;
        }



        Camera::Camera() : p_Camera(nullptr), p_Allocator(nullptr), p_Stream(nullptr), m_Requests() { }

        Camera::Camera(std::shared_ptr<libcamera::Camera> camera) : p_Camera(camera), p_Allocator(nullptr), p_Stream(nullptr), m_Requests() {
            this->p_Camera->acquire();

            std::unique_ptr<libcamera::CameraConfiguration> config = 
                this->p_Camera->generateConfiguration({ libcamera::StreamRole::Viewfinder });

            libcamera::StreamConfiguration &streamConfig = config->at(0);
            std::cout << "Default viewfinder config is: " << streamConfig.toString() << std::endl;

            streamConfig.size.width = 640;
            streamConfig.size.height = 480;
            streamConfig.pixelFormat.fromString("RGB888");

            config->validate();
            std::cout << "Validated viewfinder config is: " << streamConfig.toString() << std::endl;

            this->p_Camera->configure(config.get());

            this->p_Allocator = new libcamera::FrameBufferAllocator(this->p_Camera);

            for (libcamera::StreamConfiguration& cfg : *config) {
                if (this->p_Allocator->allocate(cfg.stream()) < 0) {
                    std::cerr << "Can't allocate buffers!" << std::endl;
                }

                unsigned int allocated = this->p_Allocator->buffers(cfg.stream()).size();
                std::cout << "Allocated " << allocated << " buffers for stream." << std::endl;
            }

            this->p_Stream = streamConfig.stream();
            const std::vector<std::unique_ptr<libcamera::FrameBuffer>>& buffers
                = this->p_Allocator->buffers(this->p_Stream);

            for (unsigned int i = 0; i < buffers.size(); i++) {
                std::unique_ptr<libcamera::Request> request = this->p_Camera->createRequest();
                if (!request) {
                    std::cerr << "Can't create request!" << std::endl;
                }

                const std::unique_ptr<libcamera::FrameBuffer>& buffer = buffers[i];
                if (request->addBuffer(this->p_Stream, buffer.get()) < 0) {
                    std::cerr << "Can't set buffer for request!" << std::endl;
                }

                libcamera::ControlList& controls = request->controls();
                controls.set(libcamera::controls::Brightness, 0.5);

                this->m_Requests.push_back(std::move(request));
            }

            this->p_Camera->requestCompleted.connect(RequestComplete);

            this->Start();
        }

        void Camera::Start() {
            this->p_Camera->start();

            for (std::shared_ptr<libcamera::Request>& request : this->m_Requests)
                p_Camera->queueRequest(request.get());
        }

        const FrameData& Camera::GetFrame() {
            return FrameData();
        }

        void Camera::Clear() {
            this->Stop();

            this->p_Allocator->free(this->p_Stream);
            delete this->p_Allocator;

            this->p_Camera->release();
            this->p_Camera.reset();
        }

    }   // GL

}   // CB
