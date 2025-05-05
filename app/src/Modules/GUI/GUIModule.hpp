#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

// libs
#include <glm/glm.hpp>

struct GLFWwindow;

namespace BB {

    namespace GL {
    
        class Renderer;

    }   // GL

    struct WindowData {
        GL::Renderer* renderer;

        glm::vec2* scale;

        SharedData* data;

        bool animated;

    };  // WindowData

    class GUIModule : public Module {
    public:
        GUIModule(SharedData* data) :
            Module(data),
            p_Renderer(nullptr),
            p_Window(nullptr),
            m_WindowData(),
            m_WindowScale(),
            m_Closed(false)
        { }
        virtual ~GUIModule() = default;

        /** @brief The initializer for the GUI. */
        virtual void Init() override;
        /** @brief Cleans up the GUI module before shutdown. */
        virtual void Shutdown() override;

        /** @brief Updates the GUI and renders a new frame. */
        virtual void Update() override;

    private:
        GUIModule(const GUIModule&) = delete;
        GUIModule operator = (const GUIModule&) = delete;

        /** @brief Draws the GUI to the screen. */
        void Render();

        /** @brief Does math to animate for demo mode. */
        void Animate();

    private:

        GL::Renderer* p_Renderer;

        GLFWwindow* p_Window;

        WindowData m_WindowData;

        glm::vec2 m_WindowScale;

        bool m_Closed;
    };

}   // BB
