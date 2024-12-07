#pragma once

// std
#include <map>

namespace  CB {

    namespace GL {
        
        class Shader;
        class Texture2D;
        class Font;

    }   // GL

    class ResourceManager {
    public:
        static GL::Shader& LoadShader(const char* source, const char* name);
        static GL::Shader& GetShader(const char* name);

        static GL::Texture2D& LoadTexture(const char* source, const char* name, bool alpha = false);
        static GL::Texture2D& GetTexture(const char* name);

        static GL::Font& LoadFont(const char* source, const char* name);
        static GL::Font& GetFont(const char* name);

        static void Clear();

    private:
        ResourceManager();

        static ResourceManager* Instance();

        GL::Shader LoadShaderFile(const char* source);
        GL::Texture2D LoadTextureFile(const char* source, bool alpha);
        GL::Font LoadFontFile(const char* source);

        /* --- */
        
        static inline ResourceManager* s_Instance = nullptr;

        std::map<const char*, GL::Shader> m_Shaders;
        std::map<const char*, GL::Texture2D> m_Textures;
        std::map<const char*, GL::Font> m_Fonts;

    };

}   // CB