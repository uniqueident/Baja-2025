/**
 * @file GUIModule.hpp
 * @author Evan F.
 * @brief The GUIModule header file. Declares the GUIModule class members and methods.
 * @version 1.0
 * @date 10-25-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "Modules/moduletemplate.hpp"

// libs
#include <glm/glm.hpp>

struct GLFWwindow;

namespace CB {

    namespace GL {
    
        class Renderer;

    }   // GL

    struct WindowData {
        GL::Renderer* renderer;

        glm::vec2* scale;

    };  // WindowData

    class GUIModule : public Module {
    public:
        GUIModule() : p_Renderer(nullptr), p_Window(nullptr), m_WindowData(), m_WindowScale(), m_Closed(false) { }
        virtual ~GUIModule() = default;

        /**
         * @brief The initializer for the GUI.
         * 
         * @param data The shared module data.
         */
        virtual void Init(SharedData* data) override;
        /** @brief Cleans up the GUI module before shutdown. */
        virtual void Shutdown() override;

        /** @brief Updates the GUI and renders a new frame. */
        virtual void Update() override;

    private:
        GUIModule(const GUIModule&) = delete;
        GUIModule operator = (const GUIModule&) = delete;

        /** @brief Draws the GUI to the screen. */
        void Render();

        /* --- */

        GL::Renderer* p_Renderer;

        GLFWwindow* p_Window;

        WindowData m_WindowData;

        glm::vec2 m_WindowScale;

        bool m_Closed;

    };

}   // CB
