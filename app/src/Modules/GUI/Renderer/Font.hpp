#pragma once

#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <map>

// libs
#include <freetype/freetype.h>
#include <glm/glm.hpp>

namespace CB {
    
    namespace GL {

        typedef GLuint VertexBuffer_ID;

        struct Character {
            Texture_ID id;

            glm::ivec2 size;
            glm::ivec2 bearing;

            long advance;

        };  // Character

        class Font {
        public:
            Font();
            Font(FT_Face face, unsigned int pixelSize);
            ~Font() = default;

        private:
            friend class Renderer;

            void DrawChar(char c, float& x, float& y, float scale, VertexBuffer_ID buffer);

            /* --- */

            std::map<char, Character> m_Characters;

        };

    }   // GL

}   // CB