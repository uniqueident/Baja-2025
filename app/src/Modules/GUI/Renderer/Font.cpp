#include "Font.hpp"

// std
#include <iostream>

// libs
#include <ft2build.h>
#include FT_FREETYPE_H

namespace BB {

    namespace GL {

        Font::Font() : m_Characters() { }

        Font::Font(FT_Face face, unsigned int pixelSize) : m_Characters() {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            FT_Set_Pixel_Sizes(face, 0, pixelSize);

            for (unsigned char i = 0; i < 128; i++) {
                if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                    std::cerr << "Failed to load Glyph!" << std::endl;
                }

                Texture_ID texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                m_Characters.insert(std::pair<char, Character>(
                    i,
                    {
                        texture,
                        { face->glyph->bitmap.width, face->glyph->bitmap.rows },
                        { face->glyph->bitmap_left, face->glyph->bitmap_top },
                        face->glyph->advance.x
                    }
                ));
            }
        }

        void Font::Clear() {
            for (auto item : m_Characters)
                glDeleteTextures(1, &item.second.id);
        }

        void Font::DrawChar(char c, float scale, VertexBuffer_ID buffer) {
            Character ch = m_Characters[c];

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            float vertices[6][4] = {
                { 0.0f, h,    0.0f, 1.0f },
                { w,    0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f, 0.0f },

                { 0.0f, h,    0.0f, 1.0f },
                { w,    h,    1.0f, 1.0f },
                { w,    0.0f, 1.0f, 0.0f },
            };

            glBindTexture(GL_TEXTURE_2D, ch.id);

            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

    }   // GL

}   // BB
