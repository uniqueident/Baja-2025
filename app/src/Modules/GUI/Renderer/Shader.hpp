#pragma once

// libs
#include <glad/gl.h>

#include <glm/glm.hpp>

namespace BB {

    namespace  GL {

        /** @brief An OpenGL shader program ID. */
        typedef GLuint Program_ID;

        /** @brief An OpenGL shader ID. */
        typedef GLuint Shader_ID;

        /** @brief A shader that is used to render data to the screen. */
        class Shader {
        private:
            /** @brief An ID from either a shader program or a shader. */
            typedef GLuint Object_ID;

            /** @brief The type of compile operation for either a shader program or a shader. */
            enum CompileType {
                PROGRAM = 0,
                SHADER  = 1,
            };

        public:
            Shader() = default;
            /**
             * @brief Construct a new Shader object.
             * 
             * @param vertexSource A string of the vertex shader source code.
             * @param fragmentSource A string of the fragment shader source code. 
             * @param geometrySource A string of the geometry shader source code.
             */
            Shader(const char* vertexSource, const char* fragmentSource);
            ~Shader() = default;

            /**
             * @brief Set a boolean in the shader.
             * 
             * @param name The name of the value variable.
             * @param value The value to set in the shader.
             */
            void SetBool  (const char* name, bool value) const;
            /**
             * @brief Set a integer in the shader.
             * 
             * @param name The name of the value variable.
             * @param value The value to set in the shader.
             */
            void SetInt   (const char* name, int value) const;
            /**
             * @brief Set a floating point value in the shader.
             * 
             * @param name The name of the value variable.
             * @param value The value to set in the shader.
             */
            void SetFloat (const char* name, float value) const;
            /**
             * @brief Set a 2D floating point vector in the shader.
             * 
             * @param name The name of the value variable.
             * @param value The value to set in the shader.
             */
            void SetVec2f (const char* name, const glm::vec2& value) const;
            /**
             * @brief Set a 3D floating point vector in the shader.
             * 
             * @param name The name of the value variable.
             * @param value The value to set in the shader.
             */
            void SetVec3f (const char* name, const glm::vec3& value) const;
            /**
             * @brief Set a 4D float point vector in the shader.
             * 
             * @param name The name of the value variable.
             * @param value The value to set in the shader.
             */
            void SetVec4f (const char* name, const glm::vec4& value) const;
            /**
             * @brief Set a 4D x 4D float point matrix in the shader.
             * 
             * @param name The name of the value variable.
             * @param value The value to set in the shader.
             */
            void SetMat4f (const char* name, const glm::mat4& value) const;

            /** @brief Use the OpenGL shader. */
            void Use();

            /**
             * @return The program ID of the shader.
             */
            inline Program_ID ID() const { return m_Program; }

        private:
            /**
             * @brief Checks for compilation erros from OpenGL.
             * 
             * @param object The object to check on.
             * @param type The type of compilation operation the object is from.
             */
            void CheckCompileErrors(Object_ID object, CompileType type);

            /* --- */

            Program_ID m_Program; /** @brief The shader program that this object controls. */

        };

    }   // GL

}   // BB