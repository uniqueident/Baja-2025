#include "Renderer.hpp"

#include "Modules/GUI/Renderer/Shader.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"

#include "Modules/GUI/Renderer/ResourceManager.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

namespace CB {

    namespace GL {

        void Renderer::Init(Shader* shader) {
            p_Shader = shader;
            p_WhiteTexture = new Texture2D();

            unsigned char textureData[] = {
                0xff, 0xff, 0xff, 0xff
            };

            p_WhiteTexture->Generate(1, 1, textureData);
        }

        void Renderer::Shutdown() {
            glDeleteVertexArrays(1, &this->m_VAO);
        }

        void Renderer::LoadData() {
            VertexBuffer_ID vbo;

            float vertices[] = { 
                // pos      // tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 

                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &this->m_VAO);
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->m_VAO);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void Renderer::DrawSprite(
            GL::Texture2D &texture,
            const glm::vec2& position, const glm::vec2& size,
            const glm::vec3& color,
            float rotate
        ) {
            this->p_Shader->Use();

            // Apply Transformations
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(position, 0.0f));  

            model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
            model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

            model = glm::scale(model, glm::vec3(size, 1.0f)); 
        
            this->p_Shader->SetMat4f("model", model);

            // Render the Quad
            this->p_Shader->SetVec3f("spriteColor", color);
        
            glActiveTexture(GL_TEXTURE0);
            texture.Bind();

            glBindVertexArray(this->m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        void Renderer::DrawQuad(
            const glm::vec3& color,
            const glm::vec2& position, const glm::vec2& size,
            float rotate
        ) {
            DrawSprite(*p_WhiteTexture, position, size, color, rotate);
        }

        void Renderer::UpdateView(unsigned int width, unsigned int height) {
            this->m_Width = width;
            this->m_Height = height;
            this->m_Scale = static_cast<float>(width) / height;

            glViewport(0, 0, width, height);

            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
            this->p_Shader->SetMat4f("projection", projection);
        }

    }   // GL

}   // CB
