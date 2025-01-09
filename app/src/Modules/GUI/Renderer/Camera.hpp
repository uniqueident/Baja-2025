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
            /**
             * @brief Construct a new Camera object.
             * 
             * @param index The index of the camera to assign.
             */
            Camera(int index);
            ~Camera();

            /**
             * @brief Get the Frame from the camera to use with OpenGL in the Renderer.
             * 
             * @return A DynamicTexture2D that is the OpenGL texture of the frame.
             */
            std::shared_ptr<DynamicTexture2D>& GetFrame();

            /**
             * @brief Starts the camera frame capturing.
             * 
             */
            void Start();
            /**
             * @brief Stops the camera frame capturing.
             * 
             */
            void Stop();

        private:
            cv::VideoCapture m_Camera; /** @brief The OpenCV camera to access and control. */
            cv::Mat m_Frame; /** @brief The OpenCV camera frame to access the frame data. */

            std::shared_ptr<DynamicTexture2D> m_Texture; /** @brief The OpenGL frame texture. */

            bool m_Started; /** @brief Records if the camera is active or not. */

        };  // Camera

    }   // GL

}   // BB
