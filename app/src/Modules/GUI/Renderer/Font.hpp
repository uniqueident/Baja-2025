#pragma once

#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <map>

// libs
#include <freetype/freetype.h>

#include <glm/glm.hpp>

namespace BB {
    
    namespace GL {

        typedef GLuint VertexBuffer_ID;

        /** @brief A single character generated to be rendered on demand. */
        struct Character {
            Texture_ID id;

            glm::ivec2 size;
            glm::ivec2 bearing;

            long advance;

        };  // Character

        class Font {
        public:
            Font();
            /**
             * @brief Construct a new Font object.
             * 
             * @param face The FreeType face to use.
             * @param pixelSize The size to generate as.
             */
            Font(FT_Face face, unsigned int pixelSize);
            ~Font() = default;

            /** @brief Deletes the character textures from OpenGL. */
            void Clear();

        private:
            friend class Renderer;

            /**
             * @brief Draw a single character on screen.
             * 
             * @param c The character to draw.
             * @param scale The text scale.
             * @param buffer The OpenGL VBO to send data to.
             */
            void DrawChar(char c, float scale, VertexBuffer_ID buffer);

            const Character& GetCharacter(char c) const {
                return this->m_Characters.at(c);
            }

            /* --- */

            std::map<char, Character> m_Characters; /** @brief A map of each character and its texture from the font. */

        };

    }   // GL

}   // BB