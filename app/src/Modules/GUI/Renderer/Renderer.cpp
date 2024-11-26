#include "Renderer.hpp"

#include "Modules/GUI/Renderer/Shader.hpp"

//std
#include <iostream>

namespace CB {

    namespace GL {

        const char *vertexShaderSource = 
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char *fragmentShaderSource = 
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n\0";

        const float vertices[] = {
            0.5f, 0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f, 0.5f, 0.0f   // top left 
        };

        unsigned int indicies[] = {
            0, 1, 3,
            1, 2, 3
        };

        void Renderer::Init() {
            m_Program = glCreateProgram();

            int success;

            // Compile and link shaders to the OpenGL program to use.

            Shader* shaders[] = {
                new Shader(vertexShaderSource, ShaderType::VERTEX_SHADER),
                new Shader(fragmentShaderSource, ShaderType::FRAGMENT_SHADER)
            };

            glAttachShader(m_Program, *shaders[1]);
            glAttachShader(m_Program, *shaders[2]);

            glLinkProgram(m_Program);

            glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[512];

                glGetProgramInfoLog(m_Program, 512, NULL, infoLog);

                std::cerr << "GL Shader Linking Failed!\n\t" << infoLog << std::endl;
            }

            delete shaders[0];
            delete shaders[1];

            // Set up the vertex data (and corresponding buffers) and configure vertex attributes

            glGenVertexArrays(1, &m_VertexArray);
            glGenBuffers(1, &m_VertexBuffer);
            glGenBuffers(1, &m_ElementBuffer);

            glBindVertexArray(m_VertexArray);

            glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindVertexArray(0);
        }

        void Renderer::Shutdown() {
            glDeleteVertexArrays(1, &m_VertexArray);
            glDeleteBuffers(1, &m_VertexBuffer);

            glDeleteProgram(m_Program);
        }

        void Renderer::Render() {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(m_Program);
            glBindVertexArray(m_VertexArray);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

    }   // GL

}   // CB
