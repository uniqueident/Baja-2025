#pragma once

#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <memory>
#include <vector>

// libs
#include <libcamera/camera.h>

namespace BB {

    namespace GL {

        struct FrameData {
            int width, height;
            
            std::shared_ptr<Texture2D> image;

        };

        class Camera {
        public:
            Camera();
            Camera(std::shared_ptr<libcamera::Camera> camera);
            ~Camera();

            void Start();
            void Stop();

            const FrameData GetFrame();

            void Clear();

            inline const std::string& ID() { return p_Camera->id(); }

        private:
            static void RequestComplete(libcamera::Request* request);



            /* --- */

            inline static std::shared_ptr<libcamera::Camera> p_Camera;
            
            std::shared_ptr<Texture2D> m_Texture;

            libcamera::Stream* p_Stream;
            libcamera::FrameBufferAllocator* p_Allocator;

            inline static std::vector<std::unique_ptr<libcamera::Request>> m_Requests;

            bool m_Active;

        };

    }   // GL

}   // BB
