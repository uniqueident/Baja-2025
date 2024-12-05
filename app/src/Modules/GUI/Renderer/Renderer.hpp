#pragma once

// libs
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace CB {

    namespace GL {

        class Shader;
        class Texture2D;

        typedef GLuint VertexArray_ID;
        typedef GLuint VertexBuffer_ID;

        typedef GLuint ElementBuffer_ID;

        class Renderer {
        public:
            static Renderer* Instance();

            void Init(Shader* shader);
            void Shutdown();

            void LoadData();

            void DrawSprite(
                GL::Texture2D &texture,
                const glm::vec2& position, const glm::vec2& size,
                const glm::vec3& color = { 1.0f, 1.0f, 1.0f },
                float rotate = 0.0f
            );

            void UpdateView(unsigned int width, unsigned int height);

        private:
            Shader* p_Shader;

            VertexArray_ID m_VAO;

            int m_Width, m_Height;

        };

    }   // GL

}   // CB
