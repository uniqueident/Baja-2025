#include "Camera.hpp"

#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <iostream>
#include <memory>
#include <unistd.h>

// libs
#include <glad/gl.h>

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>


namespace BB {

    namespace GL {

        // The OpenCV api to use.
        #ifdef RPI_PI

            #define CAMERA_API cv::CAP_V4L

        #else

            #define CAMERA_API cv::CAP_DSHOW

        #endif

        Camera::Camera() : m_Camera(), m_Frame(), m_Texture(std::make_shared<DynamicTexture2D>()), m_Started(false), m_Index(0) { }

        Camera::Camera(int index) :
            m_Camera(index, CAMERA_API),
            m_Frame(),
            m_Texture(std::make_shared<DynamicTexture2D>()),
            m_Started(false),
            m_Index(index)
        {
            if (!this->m_Camera.isOpened())
                std::cerr << "Failed to access camera!" << std::endl;

            this->m_Texture->Generate(this->m_Camera.get(cv::CAP_PROP_FRAME_WIDTH), this->m_Camera.get(cv::CAP_PROP_FRAME_HEIGHT), nullptr);
        }

        Camera::~Camera() {
            if (this->m_Started) {
                std::cerr << "Camera being released without being stopped!" << std::endl;

                Stop();
            }
        }

        std::shared_ptr<DynamicTexture2D>& Camera::GetFrame() {
            if (!this->m_Started) {
                std::cerr << "Unable to read frame from camera! Camera is either not started or unavailable." << std::endl;

                return this->m_Texture;
            }

            if (!this->m_Camera.read(this->m_Frame) || this->m_Frame.empty()) {
                std::cerr << "Failed to read camera frame!" << std::endl;

                return this->m_Texture;
            }

            cv::cvtColor(this->m_Frame, this->m_Frame, cv::COLOR_BGR2RGB);
            this->m_Texture->UpdateData(this->m_Frame.data);

            return this->m_Texture;
        }

        void Camera::Start() {
            if (!this->m_Camera.isOpened()) {
                if (!this->m_Camera.open(this->m_Index, CAMERA_API)) {
                    std::cerr << "Failed to open camera (" << this->m_Index << ")!" << std::endl;
                }
            }

            this->m_Started = true;
        }

        void Camera::Stop() {
            if (this->m_Camera.isOpened())
                this->m_Camera.release();

            this->m_Started = false;
        }

    }   // GL

}   // BB
