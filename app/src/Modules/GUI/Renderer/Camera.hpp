#pragma once

#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <cstddef>
#include <libcamera/color_space.h>
#include <libcamera/pixel_format.h>
#include <memory>
#include <vector>

// libs
#include <libcamera/camera.h>

namespace BB {

    namespace GL {

        struct CamStreamInfo {
            CamStreamInfo() : width(0), height(0), stride(0) { }

            unsigned int width;
            unsigned int height;

            unsigned int stride;

            libcamera::PixelFormat format;
            std::optional<libcamera::ColorSpace> colorSpace;

        };  // StreamInfo

        struct CamBuffer {
            CamBuffer() : fd(-1) { }

            int fd;
            size_t size;
            CamStreamInfo info;
            std::shared_ptr<Texture2D> texture;

        };  // Buffer

        class Camera {
        public:
            Camera();
            Camera(std::shared_ptr<libcamera::Camera> camera);
            ~Camera();

            void Start();
            void Stop();

            const CamBuffer& GetFrame();

            void Clear();

            inline const std::string& ID() { return p_Camera->id(); }

        private:
            static void RequestComplete(libcamera::Request* request);

            /* --- */

            inline static std::shared_ptr<libcamera::Camera> p_Camera;
            
            inline static std::shared_ptr<Texture2D> m_Texture;

            libcamera::Stream* p_Stream;
            libcamera::FrameBufferAllocator* p_Allocator;

            inline static std::vector<std::unique_ptr<libcamera::Request>> m_Requests;

            bool m_Active;

        };

    }   // GL

}   // BB
