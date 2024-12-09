#pragma once

// std
#include <libcamera/framebuffer_allocator.h>
#include <memory>

// libs
#include <libcamera/camera.h>

namespace CB {

    namespace GL {

        struct FrameData {
            int width, height;
            
            void* data;

        };

        class Camera {
        public:
            Camera();
            Camera(std::shared_ptr<libcamera::Camera> camera);
            ~Camera() = default;

            void Start();
            inline void Stop() { p_Camera->stop(); }

            const FrameData& GetFrame();

            void Clear();

            inline const std::string& ID() { return p_Camera->id(); }

        private:
            std::shared_ptr<libcamera::Camera> p_Camera;

            libcamera::FrameBufferAllocator* p_Allocator;

            libcamera::Stream* p_Stream;

            std::vector<std::shared_ptr<libcamera::Request>> m_Requests;
        };

    }   // GL

}   // CB
