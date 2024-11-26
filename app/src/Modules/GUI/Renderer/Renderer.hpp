#pragma once

#include <glad/glad.h>

namespace CB {

    namespace GL {

        typedef GLuint Program_ID;

        typedef GLuint VertexArray_ID;
        typedef GLuint VertexBuffer_ID;

        typedef GLuint ElementBuffer_ID;

        class Renderer {
        public:
            void Init();
            void Shutdown();

            void Render();

        private:
            Program_ID m_Program;

            VertexBuffer_ID m_VertexBuffer;
            VertexArray_ID m_VertexArray;

            ElementBuffer_ID m_ElementBuffer;

        };

    }   // GL

}   // CB