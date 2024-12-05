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

#include "../moduletemplate.hpp"

struct GLFWwindow;

namespace CB {

    namespace GL {
    
        class Renderer;

    }   // GL

    struct WindowData {
        GL::Renderer* renderer;

    };  // WindowData

    class GUIModule : public Module {
    public:
        GUIModule() : p_Renderer(nullptr), p_Window(nullptr), m_WindowData(), m_Closed(false) { }
        virtual ~GUIModule() = default;

        virtual void Init(SharedData* data) override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        GUIModule(const GUIModule&) = delete;
        GUIModule operator = (const GUIModule&) = delete;

        void Render();

        /* --- */

        GL::Renderer* p_Renderer;

        GLFWwindow* p_Window;

        WindowData m_WindowData;

        bool m_Closed;

    };

}   // CB
