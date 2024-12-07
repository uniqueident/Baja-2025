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
        /**
         * @brief Loads a shader file and creates a shader object.
         * 
         * @param source The source path of the file.
         * @param name The name to reference the shader.
         * @return GL::Shader& 
         */
        static GL::Shader& LoadShader(const char* source, const char* name);
        /**
         * @brief Get the Shader object.
         * 
         * @param name The name of the shader.
         * @return GL::Shader& 
         */
        static GL::Shader& GetShader(const char* name);

        /**
         * @brief Loads a texture file and creates a texture object.
         * 
         * @param source The source path of the file.
         * @param name The name to reference the texture.
         * @param alpha Enables transparent pixels on the texture.
         * @return GL::Texture2D& 
         */
        static GL::Texture2D& LoadTexture(const char* source, const char* name, bool alpha = false);
        /**
         * @brief Get the Texture object.
         * 
         * @param name The name of the texture.
         * @return GL::Texture2D& 
         */
        static GL::Texture2D& GetTexture(const char* name);

        /**
         * @brief Loads a TrueType Font and creates a font object.
         * 
         * @param source The source path of the file.
         * @param name The name to reference the texture.
         * @return GL::Font& 
         */
        static GL::Font& LoadFont(const char* source, const char* name);
        /**
         * @brief Get the Font object.
         * 
         * @param name The name of the font.
         * @return GL::Font& 
         */
        static GL::Font& GetFont(const char* name);

        /** @brief Erases all currently stored resources. */
        static void Clear();

    private:
        ResourceManager();

        /** @return The resource manager instance. */
        static ResourceManager* Instance();

        /**
         * @brief Internal loader for shader files.
         * 
         * @param source The source path of the file.
         * @return GL::Shader 
         */
        GL::Shader LoadShaderFile(const char* source);
        /**
         * @brief Internal loader for texture files.
         * 
         * @param source The source path of the file.
         * @param alpha Enables transparent pixels on the texture.
         * @return GL::Texture2D 
         */
        GL::Texture2D LoadTextureFile(const char* source, bool alpha);
        /**
         * @brief Internal loader for font files.
         * 
         * @param source The source path of the file.
         * @return GL::Font 
         */
        GL::Font LoadFontFile(const char* source);

        /* --- */
        
        static inline ResourceManager* s_Instance = nullptr;

        std::map<const char*, GL::Shader> m_Shaders;
        std::map<const char*, GL::Texture2D> m_Textures;
        std::map<const char*, GL::Font> m_Fonts;

    };

}   // CB