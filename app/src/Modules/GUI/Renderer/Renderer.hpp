#pragma once

// std
#include <string>

// libs
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace CB {

    namespace GL {

        class Shader;
        class Texture2D;
        class Font;

        typedef GLuint VertexArray_ID;
        typedef GLuint VertexBuffer_ID;

        typedef GLuint ElementBuffer_ID;

        class Renderer {
        public:
            static Renderer* Instance();

            void Init();
            void Shutdown();

            void LoadData();

            void DrawSprite(
                GL::Texture2D &texture,
                const glm::vec2& position, const glm::vec2& size,
                const glm::vec3& color = { 1.0f, 1.0f, 1.0f },
                float rotate = 0.0f
            );

            void DrawQuad(
                const glm::vec3& color,
                const glm::vec2& position, const glm::vec2& size,
                float rotate = 0.0f
            );

            void DrawText(
                const std::string& text, Font& font,
                glm::vec2 position, float size,
                const glm::vec3& color
            );

            void UpdateView(unsigned int width, unsigned int height);

        private:
            Shader* p_QuadShader;
            Shader* p_TextShader;

            VertexArray_ID m_QuadVAO;
            VertexBuffer_ID m_QuadVBO;

            VertexArray_ID m_TextVAO;
            VertexBuffer_ID m_TextVBO;

            int m_Width, m_Height;
            float m_Scale;

            Texture2D* p_WhiteTexture;

        };

    }   // GL

}   // CB
