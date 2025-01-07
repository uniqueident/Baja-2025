#include "Shader.hpp"

#include "Modules/GUI/Renderer/Renderer.hpp"

// std
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>

// libs
#include <glm/gtc/type_ptr.hpp>

namespace BB {

    namespace GL {
    
        Shader::Shader(const char* vertexSource, const char* fragmentSource, const char* geometrySource) : m_Program(0) {

            // std::cout << vertexSource   << std::endl;
            // std::cout << fragmentSource << std::endl;
            // std::cout << geometrySource << std::endl;

            Shader_ID vertex, fragment, geometry;

            // Vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);

            glShaderSource(vertex, 1, &vertexSource, NULL);
            glCompileShader(vertex);

            CheckCompileErrors(vertex, SHADER);

            // Fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);

            glShaderSource(fragment, 1, &fragmentSource, NULL);
            glCompileShader(fragment);

            CheckCompileErrors(fragment, SHADER);

            // Geometry Shader If Given
            // if (geometrySource != nullptr) {
            //     geometry = glCreateShader(GL_GEOMETRY_SHADER);

            //     glShaderSource(geometry, 1, &geometrySource, NULL);
            //     glCompileShader(geometry);

            //     CheckCompileErrors(geometry, SHADER);
            // }

            // Create The Shader Program
            m_Program = glCreateProgram();

            glAttachShader(m_Program, vertex);
            glAttachShader(m_Program, fragment);
            // if (geometrySource != nullptr)
            //     glAttachShader(m_Program, geometry);

            glLinkProgram(m_Program);

            CheckCompileErrors(m_Program, PROGRAM);
            
            // Clean Up Unecessary Data
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            // if (geometrySource != nullptr)
            //     glDeleteShader(geometry);
        }

        void Shader::SetBool(const char* name, bool value) const {
            glUniform1i(glGetUniformLocation(m_Program, name), static_cast<int>(value));
        }

        void Shader::SetInt(const char* name, int value) const {
            glUniform1i(glGetUniformLocation(m_Program, name), value);
        }

        void Shader::SetFloat(const char* name, float value) const {
            glUniform1f(glGetUniformLocation(this->m_Program, name), value);
        }

        void Shader::SetVec2f(const char* name, const glm::vec2& value) const {
            glUniform2f(glGetUniformLocation(this->m_Program, name), value.x, value.y);
        }

        void Shader::SetVec3f(const char* name, const glm::vec3& value) const {
            glUniform3f(glGetUniformLocation(this->m_Program, name), value.x, value.y, value.z);
        }

        void Shader::SetVec4f(const char* name, const glm::vec4& value) const {
            glUniform4f(glGetUniformLocation(this->m_Program, name), value.x, value.y, value.z, value.w);
        }

        void Shader::SetMat4f(const char* name, const glm::mat4& value) const {
            glUniformMatrix4fv(glGetUniformLocation(this->m_Program, name), 1, false, glm::value_ptr(value));
        }

        void Shader::Use() {
            glUseProgram(this->m_Program);
        }

        void Shader::CheckCompileErrors(Object_ID object, CompileType type) {
            int success;
            char infoLog[1024];

            switch (type) {
                case PROGRAM:
                    glGetProgramiv(object, GL_LINK_STATUS, &success);
                    if (!success) {
                        glGetProgramInfoLog(object, 1024, NULL, infoLog);

                        std::cerr << "Shader Program Error:\n\t" << infoLog << std::endl;
                    }
                    break;

                case SHADER:
                    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
                    if (!success) {
                        glGetShaderInfoLog(object, 1024, NULL, infoLog);

                        std::cerr << "Shader Compile Error:\n\t" << infoLog << std::endl;
                    }
            }
        }

    }   // GL

}   // BB