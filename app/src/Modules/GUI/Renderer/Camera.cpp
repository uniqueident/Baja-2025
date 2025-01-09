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


namespace BB {

    namespace GL {

        // The set frame width.
        #define CAMERA_VIEW_WIDTH 1280
        // The set frame height.
        #define CAMERA_VIEW_HEIGHT 720

        Camera::Camera() : m_Camera(), m_Frame(), m_Texture(std::make_shared<DynamicTexture2D>()), m_Started(false) { }

        Camera::Camera(int index) : m_Camera(index, cv::CAP_V4L2), m_Frame(), m_Texture(std::make_shared<DynamicTexture2D>()), m_Started(false) {
            if (!this->m_Camera.isOpened())
                std::cerr << "Failed to access camera!" << std::endl;

            this->m_Texture->Generate(this->m_Camera.get(cv::CAP_PROP_FRAME_WIDTH), this->m_Camera.get(cv::CAP_PROP_FRAME_HEIGHT), nullptr);
        }

        Camera::~Camera() {
            
        }

        std::shared_ptr<DynamicTexture2D>& Camera::GetFrame() {
            this->m_Camera.read(this->m_Frame);

            unsigned char* data = this->m_Frame.data;

            this->m_Texture->UpdateData(data);

            return this->m_Texture;
        }

        void Camera::Start() {
            if (this->m_Started == true)
                std::cerr << "Camera start called after camera is already started!" << std::endl;

            
        }

        void Camera::Stop() {
            this->m_Started = false;
        }

    }   // GL

}   // BB
