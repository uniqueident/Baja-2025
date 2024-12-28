#include "Renderer.hpp"

#include "Modules/GUI/Renderer/Shader.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"
#include "Modules/GUI/Renderer/Font.hpp"
#include "Modules/GUI/Renderer/Camera.hpp"

#include "Modules/GUI/Renderer/ResourceManager.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

namespace BB {

    namespace GL {

        void Renderer::Init() {
            p_QuadShader = &ResourceManager::LoadShader("../../assets/Shaders/Sprite.glsl", "Sprite");
            p_QuadShader->Use();
            p_QuadShader->SetInt("image", 0);

            p_TextShader = &ResourceManager::LoadShader("../../assets/Shaders/Text.glsl", "Text");
            p_TextShader->Use();
            p_TextShader->SetInt("text", 0);

            p_WhiteTexture = new Texture2D();
        }

        void Renderer::Shutdown() {
            glDeleteBuffers(1, &this->m_TextVBO);
            glDeleteVertexArrays(1, &this->m_TextVAO);

            glDeleteBuffers(1, &this->m_QuadVBO);
            glDeleteVertexArrays(1, &this->m_QuadVAO);

            delete p_WhiteTexture;
        }

        void Renderer::LoadData() {
            unsigned char textureData[] = {
                0xff, 0xff, 0xff, 0xff
            };

            p_WhiteTexture->Generate(1, 1, textureData);

            // Quad Vertex Array / Buffer

            float vertices[] = { 
                // pos      // tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 

                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &this->m_QuadVAO);
            glGenBuffers(1, &m_QuadVBO);

            glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->m_QuadVAO);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            // Text Vertex Array / Buffer

            glGenVertexArrays(1, &this->m_TextVAO);
            glGenBuffers(1, &this->m_TextVBO);

            glBindBuffer(GL_ARRAY_BUFFER, this->m_TextVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

            glBindVertexArray(this->m_TextVAO);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void Renderer::DrawSprite(
            GL::Texture2D &texture,
            const glm::vec2& position, const glm::vec2& size, float rotate,
            const glm::vec3& color
        ) {
            this->p_QuadShader->Use();

            // Apply Transformations
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(position, 0.0f));  

            model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
            model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

            model = glm::scale(model, glm::vec3(size, 1.0f)); 
        
            this->p_QuadShader->SetMat4f("model", model);

            // Render the Quad
            this->p_QuadShader->SetVec3f("spriteColor", color);
        
            glActiveTexture(GL_TEXTURE0);
            texture.Bind();

            glBindVertexArray(this->m_QuadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            glBindVertexArray(0);
        }

        void Renderer::DrawQuad(
            const glm::vec3& color,
            const glm::vec2& position, const glm::vec2& size, float rotate
        ) {
            DrawSprite(*p_WhiteTexture, position, size, rotate, color);
        }

        void Renderer::DrawText(
            const std::string& text, Font& font,
            glm::vec2 position, float size,
            const glm::vec3& color
        ) {
            p_TextShader->Use();
            p_TextShader->SetVec3f("textColor", color);

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(m_TextVAO);

            for (unsigned int i = 0; i < text.length(); i++) {
                font.DrawChar(text[i], position.x, position.y, size, this->m_TextVBO);
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Renderer::DrawCam(
            Camera& cam,
            glm::vec2 position, glm::vec2 size, float rotate
        ) {
            FrameData data = cam.GetFrame();

            if (size.x == 0.0f)
                DrawSprite(*data.image, position, { data.width, size.y }, rotate);
            else if (size.y == 0.0f)
                DrawSprite(*data.image, position, { size.x, data.height }, rotate);
            else if (size.x == 0.0f && size.y == 0.0f)
                DrawSprite(*data.image, position, { data.width, data.height }, rotate);
            else
                DrawSprite(*data.image, position, size, rotate);
        }

        void Renderer::UpdateView(unsigned int width, unsigned int height) {
            glViewport(0, 0, width, height);

            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
            this->p_QuadShader->Use();
            this->p_QuadShader->SetMat4f("projection", projection);
            this->p_TextShader->Use();
            this->p_TextShader->SetMat4f("projection", projection);
        }

    }   // GL

}   // BB
