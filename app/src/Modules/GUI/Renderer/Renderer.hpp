#pragma once

// libs
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace CB {

    namespace GL {

        class Shader;
        class Texture2D;

        typedef GLuint VertexArray_ID;
        typedef GLuint VertexBuffer_ID;

        typedef GLuint ElementBuffer_ID;

    }   // GL

    // https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites

    class Renderer2D {
    public:
        static void DrawSprite(
            GL::Texture2D &texture,
            const glm::vec2& position, const glm::vec2& size = { 10.0f, 10.0f },
            const glm::vec3& color = { 1.0f, 1.0f, 1.0f },
            float rotate = 0.0f
        );

    };

    namespace GL {

        class Renderer {
        public:
            static Renderer& Instance();

            void Init();
            void Shutdown();

            void Render();

        private:
            VertexBuffer_ID m_VertexBuffer;
            VertexArray_ID m_VertexArray;

            ElementBuffer_ID m_ElementBuffer;

            glm::mat4 m_Projection;

        };

    }   // GL

}   // CB
