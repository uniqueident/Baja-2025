#include "Texture.hpp"

namespace CB {

    namespace GL {

        Texture2D::Texture2D() : m_ID(0), m_Width(0), m_Height(0), m_InternalFormat(GL_RGB), m_Format(GL_RGB), m_WrapS(GL_REPEAT), m_WrapT(GL_REPEAT), m_FilterMin(GL_LINEAR), m_FilterMax(GL_LINEAR) {
            glGenTextures(1, &this->m_ID);
        }
        
        void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data) {
            this->m_Width = width;
            this->m_Height = height;

            // Create the Texture
            glBindTexture(GL_TEXTURE_2D, this->m_ID);
            glTexImage2D(GL_TEXTURE_2D, 0, this->m_InternalFormat, width, height, 0, this->m_Format, GL_UNSIGNED_BYTE, data);

            // Set the Texture Wrap and Filter Modes
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_WrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_WrapT);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_FilterMin);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_FilterMax);

            // Unbind the Texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture2D::Bind() const {
            glBindTexture(GL_TEXTURE_2D, this->m_ID);
        }

        void Texture2D::SetAlpha(bool alpha) {
            if (alpha) {
                this->m_InternalFormat = GL_RGBA;
                this->m_Format = GL_RGBA;
            }
            else {
                this->m_InternalFormat = GL_RGB;
                this->m_Format = GL_RGB;
            }
        }

    }   // GL

}   // CB