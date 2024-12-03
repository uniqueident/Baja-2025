#include "ResourceManager.hpp"

#include "Modules/GUI/Renderer/Shader.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"

// libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// std
#include <iostream>

namespace CB {

    GL::Shader& ResourceManager::LoadShader(const char* source, const char* name) {
        return (Instance()->m_Shaders[name] = Instance()->LoadShaderFile(source));
    }

    GL::Shader& ResourceManager::GetShader(const char* name) {
        return Instance()->m_Shaders[name];
    }

    GL::Texture2D& ResourceManager::LoadTexture(const char* source, const char* name) {
        return (Instance()->m_Textures[name] = Instance()->LoadTextureFile(source));
    }

    GL::Texture2D& ResourceManager::GetTexture(const char* name) {
        return Instance()->m_Textures[name];
    }

    void ResourceManager::Clear() {
        Instance()->m_Shaders.clear();
        Instance()->m_Textures.clear();
    }

    ResourceManager::ResourceManager() : m_Shaders(), m_Textures() { }

    ResourceManager* ResourceManager::Instance() {
        if (s_Instance != nullptr)
            return s_Instance;

        return (s_Instance = new ResourceManager);
    }

    #define MAX_LINE_LENGTH 96U

    enum ShaderType {
        VERTEX   = 0,
        FRAGMENT = 1,
        GEOMETRY = 2,
    };

    GL::Shader ResourceManager::LoadShaderFile(const char* source) {
        FILE* fp = fopen(source, "r");

        if (fp == NULL) {
            std::cerr << "Failed To Open File! (" << source << ")" << std::endl;
        }

        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
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
                    case 'g':
                        type = GEOMETRY;
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
                case GEOMETRY:
                    geometryCode.append(buffer);
                    break;
            }
        }

        fclose(fp);

        return GL::Shader(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.c_str());
    }

    GL::Texture2D ResourceManager::LoadTextureFile(const char* source) {
        GL::Texture2D texture;

        // Load Image
        int width, height, nChannels;
        unsigned char* data = stbi_load(source, &width, &height, &nChannels, 0);

        // Generate Texture
        texture.Generate(width, height, data);

        // Free stbi Image
        stbi_image_free(data);

        return texture;
    }

}   // CB