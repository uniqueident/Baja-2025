#pragma once

// libs
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace CB {

    namespace  GL {

        typedef GLuint Program_ID;

        typedef GLuint Shader_ID;

        class Shader {
        private:
            typedef GLuint Object_ID;

            enum CompileType {
                PROGRAM = 0,
                SHADER  = 1,
            };

        public:
            Shader() = default;
            Shader(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
            ~Shader();

            void SetBool  (const char* name, bool value) const;
            void SetInt   (const char* name, int value) const;
            void SetFloat (const char* name, float value) const;
            void SetVec2f (const char* name, const glm::vec2& value) const;
            void SetVec3f (const char* name, const glm::vec3& value) const;
            void SetVec4f (const char* name, const glm::vec4& value) const;
            void SetMat4f (const char* name, const glm::mat4& value) const;

            void Use();

        private:
            void CheckCompileErrors(Object_ID object, CompileType type);

            /* --- */

            Program_ID m_Program;

        };

    }   // GL

}   // CB