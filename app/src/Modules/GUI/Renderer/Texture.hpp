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
            Texture_ID m_ID; /** @brief The texture ID in OpenGL. */

            unsigned int m_Width; /** @brief The texture width. */
            unsigned int m_Height; /** @brief The texture height. */

            InternalFormat m_InternalFormat; /** @brief The internal OpenGL format. */
            ImageFormat m_Format; /** @brief The OpenGL texture format. */

            WrappingMode m_WrapS; /** @brief The OpenGL texuture side wrapping mode. */
            WrappingMode m_WrapT; /** @brief The OpenGL texuture top wrapping mode. */

            FilterMode m_FilterMin; /** @brief The OpenGL texture minimizing filter. */
            FilterMode m_FilterMax; /** @brief The OpenGL texture maximizing filter. */

        };  // Texture2D



        class DynamicTexture2D : public Texture2D {
        public:
            /**
             * @brief Updates the OpenGL texture data.
             * 
             * @param data The data to load to the texture.
             */
            void UpdateData(unsigned char* data);

        };  // DynamicTexture2D

    }   // GL

}   // BB
