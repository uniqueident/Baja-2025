#include "Camera.hpp"

#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <iostream>
#include <memory>
#include <opencv2/videoio.hpp>
#include <unistd.h>

// libs
#include <epoxy/egl.h>


namespace BB {

    namespace GL {

        #define CAMERA_VIEW_WIDTH 1280
        #define CAMERA_VIEW_HEIGHT 720

        Camera::Camera() { }

        Camera::Camera(int index) : m_Camera(index), m_Frame(), m_Texture(std::make_shared<DynamicTexture2D>()), m_Started(false) {
            if (!this->m_Camera.isOpened())
                std::cerr << "Failed to access camera!" << std::endl;

            this->m_Texture->Generate(this->m_Camera.get(cv::CAP_PROP_FRAME_WIDTH), this->m_Camera.get(cv::CAP_PROP_FRAME_HEIGHT), nullptr);

            // this->m_Buffer.resize(this->m_Frame.cols * this->m_Frame.rows * this->m_Frame.channels());
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
            this->m_Started = true;
        }

        void Camera::Stop() {
            this->m_Started = false;
        }

    }   // GL

}   // BB
