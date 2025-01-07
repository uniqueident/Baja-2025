#pragma once

// libs
#include <glad/gl.h>

namespace BB {

    namespace GL {

        typedef GLuint Texture_ID;

        typedef GLuint InternalFormat;
        typedef GLuint ImageFormat;

        typedef GLuint WrappingMode;

        typedef GLuint FilterMode;
    
        class Texture2D {
        public:
            Texture2D();
            ~Texture2D() = default;

            /**
             * @brief Generates the texture for OpenGL.
             * 
             * @param width The texture width.
             * @param height The texture height.
             * @param data The pixel data of the texture.
             */
            void Generate(unsigned int width, unsigned int height, unsigned char* data);

            /** @brief Binds the texture in OpenGL. */
            void Bind() const;

            /**
             * @brief Enables or disables transparency for texture generation.
             * 
             * @param alpha True if transparent, false otherwise.
             */
            void SetAlpha(bool alpha);

            /** @return The texture ID. */
            inline Texture_ID& ID() { return m_ID; }

        protected:
            Texture_ID m_ID;

            unsigned int m_Width;
            unsigned int m_Height;

            InternalFormat m_InternalFormat;
            ImageFormat m_Format;

            WrappingMode m_WrapS;
            WrappingMode m_WrapT;

            FilterMode m_FilterMin;
            FilterMode m_FilterMax;

        };

        class DynamicTexture2D : public Texture2D {
        public:
            void UpdateData(unsigned char* data);

        };  // DynamicTexture2D

    }   // GL

}   // BB
