#pragma once

// std
#include <string>

// libs
#include <glad/gl.h>

#include <glm/glm.hpp>

namespace BB {

    namespace GL {

        class Shader;
        class Texture2D;
        class Font;
        class Camera;

        typedef GLuint VertexArray_ID;
        typedef GLuint VertexBuffer_ID;

        typedef GLuint ElementBuffer_ID;

        class Renderer {
        public:
            /** @brief Initializes shaders and base variables for the renderer. */
            void Init();
            /** @brief Cleans up the renderer for shutdown. */
            void Shutdown();

            /** @brief Loads the data necessary for rendering. */
            void LoadData();

            /**
             * @brief Draws a 2D image on a quad.
             * 
             * @param texture The texture (image) to render.
             * @param position The position on screen.
             * @param size The size on screen.
             * @param rotate The rotation on screen.
             * @param color The color to tint on screen.
             */
            void DrawSprite(
                GL::Texture2D &texture,
                const glm::vec2& position, const glm::vec2& size, float rotate = 0.0f,
                const glm::vec3& color = { 1.0f, 1.0f, 1.0f }
            );

            /**
             * @brief Draws a colored 2D quad.
             * 
             * @param color The color on screen.
             * @param position The position on screen.
             * @param size The size on screen.
             * @param rotate The rotation on screen.
             */
            void DrawQuad(
                const glm::vec3& color,
                const glm::vec2& position, const glm::vec2& size, float rotate = 0.0f
            );

            /**
             * @brief Draws a string of text the given font.
             * 
             * @param text The text string to render.
             * @param font The font to render the text as.
             * @param position The position of the text.
             * @param size The size of the text (relative to 24pt font size).
             * @param color The color of the text.
             */
            void DrawText(
                const std::string& text, Font& font,
                glm::vec2 position, float size,
                const glm::vec3& color
            );

            /**
             * @brief Draws a camera frame as a textured quad.
             * 
             * @param cam The camera to capture from.
             * @param position The position on screen.
             * @param size The size on screen.
             * @param rotate The rotation on screen.
             */
            void DrawCam(
                Camera& cam,
                glm::vec2 position, glm::vec2 size, float rotate = 0.0f
            );

            /**
             * @brief Update the OpenGL view and view projection matrix of the screen.
             * 
             * @param width The new window width.
             * @param height The new window height.
             */
            void UpdateView(unsigned int width, unsigned int height);

        private:
            Shader* p_QuadShader;
            Shader* p_TextShader;

            VertexArray_ID m_QuadVAO;
            VertexBuffer_ID m_QuadVBO;

            VertexArray_ID m_TextVAO;
            VertexBuffer_ID m_TextVBO;

            Texture2D* p_WhiteTexture;

        };

    }   // GL

}   // BB
