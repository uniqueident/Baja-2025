#pragma once

// libs
#include <glad/glad.h>

namespace CB {

    namespace GL {

        typedef GLuint Texture_ID;

        typedef GLuint InternalFormat;
        typedef GLuint ImageFormat;

        typedef GLuint WrappingMode;

        typedef GLuint FilterMode;
    
        class Texture2D {
        public:
            Texture2D();
            ~Texture2D();

            void Generate(unsigned int width, unsigned int heightn, unsigned char* data);

            void Bind() const;

        private:
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

    }   // GL

}   // CB
