#pragma once

// libs
#include <glad/glad.h>

namespace CB {

    namespace  GL {

        enum ShaderType {
            VERTEX_SHADER = GL_VERTEX_SHADER,
            FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        };

        typedef GLuint ShaderID;

        class Shader {
        public:
            Shader(const char* source, ShaderType type);
            ~Shader();

            operator GLuint() const {
                return m_Shader;
            }

        private:
            ShaderID m_Shader;

        };

    }   // GL

}   // CB