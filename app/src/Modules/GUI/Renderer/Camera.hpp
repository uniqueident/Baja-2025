#pragma once

#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <memory>

// libs
#include <opencv2/videoio.hpp>

namespace BB {

    namespace GL {

        class Camera {
        public:
            Camera();
            Camera(int index);
            ~Camera();

            std::shared_ptr<DynamicTexture2D>& GetFrame();

            void Start();
            void Stop();

        private:


            /* --- */

            cv::VideoCapture m_Camera;
            cv::Mat m_Frame;

            std::shared_ptr<DynamicTexture2D> m_Texture;

            bool m_Started;

        };

    }   // GL

}   // BB
