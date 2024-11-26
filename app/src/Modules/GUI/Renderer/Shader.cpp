#include "Shader.hpp"

// std
#include <iostream>

namespace CB {

    namespace GL {
    
        Shader::Shader(const char* source, ShaderType type) : m_Shader(0) {
            m_Shader = glCreateShader(type);

            glShaderSource(m_Shader, 1, &source, NULL);
            glCompileShader(m_Shader);

            int success;
            glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                
                glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);

                std::cerr << "GL Shader Compilation Failed!\n\t" << infoLog << std::endl;
            }
        }

        Shader::~Shader() {
            glDeleteShader(m_Shader);
        }

    }   // GL

}   // CB