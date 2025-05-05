#include "ResourceManager.hpp"

#include "Modules/GUI/Renderer/Shader.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"
#include "Modules/GUI/Renderer/Font.hpp"
#include "Modules/GUI/Renderer/Camera.hpp"
#include "Modules/GUI/Renderer/Renderer.hpp"

// std
#include <iostream>

// libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace BB {

    ResourceManager::~ResourceManager() {

    }

    GL::Shader& ResourceManager::LoadShader(const char* source, const char* name) {
        return (Instance()->m_Shaders[name] = Instance()->LoadShaderFile(source));
    }

    GL::Shader& ResourceManager::GetShader(const char* name) {
        return Instance()->m_Shaders[name];
    }

    GL::Texture2D& ResourceManager::LoadTexture(const char* source, const char* name, bool alpha) {
        return (Instance()->m_Textures[name] = Instance()->LoadTextureFile(source, alpha));
    }

    GL::Texture2D& ResourceManager::GetTexture(const char* name) {
        return Instance()->m_Textures[name];
    }

    GL::Font& ResourceManager::LoadFont(const char *source, const char *name) {
        return (Instance()->m_Fonts[name] = Instance()->LoadFontFile(source));
    }

    GL::Font& ResourceManager::GetFont(const char *name) {
        return Instance()->m_Fonts[name];
    }

    GL::Camera& ResourceManager::LoadCamera(int index) {
        return (Instance()->m_Cameras[index] = Instance()->LoadCameraFromManager(index));
    }

    GL::Camera& ResourceManager::GetCamera(int index) {
        return Instance()->m_Cameras[index];
    }

    void ResourceManager::Clear() {
        for (auto item : Instance()->m_Shaders)
            glDeleteProgram(item.second.ID());
        for (auto item : Instance()->m_Textures)
            glDeleteTextures(1, &item.second.ID());
        for (auto item : Instance()->m_Fonts)
            item.second.Clear();

        Instance()->m_Shaders.clear();
        Instance()->m_Textures.clear();
        Instance()->m_Fonts.clear();
        Instance()->m_Cameras.clear();
    }

    ResourceManager::ResourceManager() : m_Shaders(), m_Textures(), m_Fonts(), m_Cameras() {

    }

    ResourceManager* ResourceManager::Instance() {
        if (s_Instance != nullptr)
            return s_Instance;

        return (s_Instance = new ResourceManager);
    }

    #define MAX_LINE_LENGTH 96U

    enum ShaderType {
        VERTEX   = 0,
        FRAGMENT = 1,
    };

    GL::Shader ResourceManager::LoadShaderFile(const char* source) {
        FILE* fp = fopen(source, "r");

        if (fp == NULL) {
            std::cerr << "Failed To Open File! (" << source << ")" << std::endl;
        }

        std::string vertexCode;
        std::string fragmentCode;
        char buffer[MAX_LINE_LENGTH];
        ShaderType type;

        // Parse The Shader File Into Its Components
        while (std::fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
            if (buffer[0] == '~') {
                switch (buffer[1]) {
                    case 'v':
                        type = VERTEX;
                        break;
                    case 'f':
                        type = FRAGMENT;
                        break;
                }

                continue;
            }

            switch (type) {
                case VERTEX:
                    vertexCode.append(buffer);
                    break;
                case FRAGMENT:
                    fragmentCode.append(buffer);
                    break;
            }
        }

        fclose(fp);

        return GL::Shader(vertexCode.c_str(), fragmentCode.c_str());
    }

    GL::Texture2D ResourceManager::LoadTextureFile(const char* source, bool alpha) {
        GL::Texture2D texture;
        texture.SetAlpha(alpha);

        // Load Image
        int width, height, nChannels;
        unsigned char* data = stbi_load(source, &width, &height, &nChannels, 0);

        // Generate Texture
        texture.Generate(width, height, data);

        // Free stbi Image
        stbi_image_free(data);

        return texture;
    }

    GL::Font ResourceManager::LoadFontFile(const char* source) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "Could not init FreeType librar" << std::endl;
        }

        FT_Face face;
        if (FT_New_Face(ft, source, 0, &face)) {
            std::cerr << "Failed to load font (" << source << ")!" << std::endl;
        }

        GL::Font font(face, 128);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return font;
    }

    GL::Camera ResourceManager::LoadCameraFromManager(int index) {
        return GL::Camera(index);
    }

}   // BB