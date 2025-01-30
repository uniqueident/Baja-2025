#include "GUIModule.hpp"

#include "Core/SharedData.hpp"
#include "Modules/GUI/Renderer/Camera.hpp"
#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/ResourceManager.hpp"

// std
#include <cstddef>
#include <iostream>
#include <sstream>

// libs
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BB {

    #define SCREEN_WIDTH 1024
    #define SCREEN_HEIGHT 600

    void glfwErrorCallback(int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

    void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {        
        WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        
        data->renderer->UpdateView(width, height);
        data->scale->x = width / static_cast<float>(SCREEN_WIDTH);
        data->scale->y = height / static_cast<float>(SCREEN_HEIGHT);
    }



    void GUIModule::Init(SharedData * data) {
        this->p_Data = data;
        this->m_Closed = false;
        this->m_WindowScale = { 1.0f, 1.0f };

        if (!glfwInit())
            std::cerr << "Failed to initialize GLFW!" << std::endl;

        glfwSetErrorCallback(glfwErrorCallback);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        // Get the connected monitors
        int monitorCount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

        // If there is only one monitor
        if (monitorCount == 1) {
            const GLFWvidmode* vidMode = glfwGetVideoMode(monitors[0]);
            
            // Check if it's a monitor that fullscreen is prefered.
            // NOTE: This may need to be changed to assume that it should always be fullscreened if there is only one monitor.
            if (vidMode->width == SCREEN_WIDTH && vidMode->height == SCREEN_HEIGHT) {
                this->p_Window = glfwCreateWindow(vidMode->width, vidMode->height, "Buggie Bug Dashboard", nullptr, nullptr);
                
                glfwSetWindowMonitor(this->p_Window, monitors[monitorCount - 1], 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
            }
            else
                this->p_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Buggie Bug Dashboard", nullptr, nullptr);
        }
        // If there are multiple monitors, assumes the last one is the correct monitor (AKA non-primary screen).
        else {
            const GLFWvidmode* vidMode = glfwGetVideoMode(monitors[monitorCount - 1]);

            this->p_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Buggie Bug Dashboard", nullptr, nullptr);

            glfwSetWindowMonitor(this->p_Window, monitors[monitorCount - 1], 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
        }

        // Error if the window failed to be made.
        if (!this->p_Window) {
            std::cerr << "Failed to create GLFW Window or OpenGL context!" << std::endl;

            glfwTerminate();
            this->m_Closed = true;

            std::cerr << "GLFW has been closed, please check for issues and run again." << std::endl;
            return;
        }

        glfwMakeContextCurrent(this->p_Window);
        
        if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD loader!" << std::endl;

            this->Shutdown();
            this->m_Closed = true;

            std::cerr << "GLFW has been closed, please check for issues and run again." << std::endl;
            return;
        }

        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(this->p_Window, glfwFramebufferSizeCallback);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->p_Renderer = new GL::Renderer;
        this->p_Renderer->Init();
        this->p_Renderer->UpdateView(SCREEN_WIDTH, SCREEN_HEIGHT);
        this->p_Renderer->LoadData();

        this->m_WindowData.renderer = this->p_Renderer;
        this->m_WindowData.scale = &this->m_WindowScale;
        glfwSetWindowUserPointer(this->p_Window, &this->m_WindowData);

        // Load Textures Here //
        
        // ResourceManager::LoadTexture("../../assets/awesomeface.png", "Face", true);
        ResourceManager::LoadTexture("../../assets/dashboardUI.png", "Dashboard", true);
        ResourceManager::LoadTexture("../../assets/dashboardGearShift.png", "GearShift", true);

        ResourceManager::LoadFont("../../assets/Fonts/ComicNeue-Bold.ttf", "ComicNeue");

        ResourceManager::LoadCamera();
        ResourceManager::GetCamera().Start();
    }

    void GUIModule::Shutdown() {
        ResourceManager::GetCamera(0).Stop();

        this->p_Renderer->Shutdown();
        delete this->p_Renderer;

        ResourceManager::Clear();

        glfwDestroyWindow(this->p_Window);

        glfwTerminate();
    }

    void GUIModule::Update() {
        if (glfwWindowShouldClose(this->p_Window)) {
            this->p_Data->running = false;

            return;
        }
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Render();

        glfwSwapBuffers(this->p_Window);
    }

    void GUIModule::Render() {
        // Render UI Here //
        glm::vec2 screenSize = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) * this->m_WindowScale;

        // p_Renderer->DrawSprite(
        //     ResourceManager::GetTexture("Face"),
        //     { 200.0f, 100.0f},
        //     { 200.0f, 300.0f },
        //     30.0f,
        //     { 0.0f, 0.0f, 1.0f }
        // );

        this->p_Renderer->DrawQuad(
            { 0.8f, 0.8f, 0.8f },
            { 0.0f, 0.0f},
            screenSize
        );
        //dont touch this it is perfect how it is :3
        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Dashboard"),
            { 0.0f, 0.0f},
            screenSize // I touched it :^)
        );

        // The gearShiftPos will need to be adjusted if the image is changed but it works :)
        glm::vec2 gearShiftPos(85.0f, 275.0f);
        switch (this->p_Data->gearPosition) {
            case PARK:
                break;

            case REVERSE:
                gearShiftPos.x -= 37.0f;
                break;

            case NEUTRAL:
                gearShiftPos.x -= 37.0f * 2.0f;
                break;

            case DRIVE:
                gearShiftPos.x -= 38.0f * 3.0f;
                break;

            case ONE:
                gearShiftPos.x -= 36.0f * 4.0f;
                break;

            case TWO:
                gearShiftPos.x -= 35.0f * 5.0f;
                break;
        }

        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("GearShift"),
            gearShiftPos * this->m_WindowScale,
            screenSize
        );
        
        //string version of the pi heat float
        std::stringstream str_Data << this->p_Data->pi_Heat << " %";

        //pi heat represented as text on the GUI
        this->p_Renderer->DrawText(
            str_Data.str(),
            ResourceManager::GetFont("ComicNeue"),
            this->m_WindowScale * glm::vec2(140.0f, 52.0f),
            1.2f,
            {0.82f, 0.106f, 0.106f }
        );

        // this->p_Renderer->DrawCam(
        //     ResourceManager::GetCamera(0), 
        //     { 50.0f, 50.0f },
        //     { 640.0f, 480.0f }
        // );
    }

}   // BB