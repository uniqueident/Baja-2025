#include "GUIModule.hpp"

#include "Core/SharedData.hpp"

#include "Core/Time.hpp"
#include "Modules/GUI/Renderer/Camera.hpp"
#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/ResourceManager.hpp"

// std
#include <algorithm>
#include <iostream>
#include <random>
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

    void key_callback(GLFWwindow* window, int key, [[gnu::unused]] int scancode, int action, [[gnu::unused]] int mods) {
        WindowData* ref = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_E && action == GLFW_PRESS){
            ref->data->fuel = 1.5f;
        }

        if (key == GLFW_KEY_W && action == GLFW_REPEAT){
            ref->data->milesPerHour += 1;
        }

        if (key == GLFW_KEY_S && action == GLFW_REPEAT){
            ref->data->milesPerHour -= 1;
            if(ref->data->milesPerHour < 0){
                ref->data->gearPosition = REVERSE;
            }
        }
        
        if (key == GLFW_KEY_P && action == GLFW_PRESS){
            ref->data->gearPosition = PARK;
        }

        if (key == GLFW_KEY_D && action == GLFW_PRESS){
            ref->data->gearPosition = DRIVE;
        }

        if (key == GLFW_KEY_1 && action == GLFW_PRESS){
            ref->data->gearPosition = ONE;
        }

        if (key == GLFW_KEY_2 && action == GLFW_PRESS){
            ref->data->gearPosition = TWO;
        }

        if (key == GLFW_KEY_N && action == GLFW_PRESS){
            ref->data->gearPosition = NEUTRAL;
        }

        if (key == GLFW_KEY_R && action == GLFW_PRESS){
            ref->data->gearPosition = REVERSE;
        }

        if (key == GLFW_KEY_Q && action == GLFW_PRESS){
            ref->data->fuel = 0.0f;
            ref->data->CVT_Heat = 0.0f;
            ref->data->pi_Heat = 0.0f;
            ref->data->engineRPM = 0;
            ref->data->milesPerHour = 0;
            ref->data->gearPosition = PARK;
        }

        if (key == GLFW_KEY_A && action == GLFW_REPEAT){
            ref->data->engineRPM += 10;
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            ref->animated = !ref->animated;

        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
            ref->data->running = false;
        }
    }



    void GUIModule::Init() {
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

        //demo key input thingy
        glfwSetKeyCallback(this->p_Window, key_callback);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->p_Renderer = new GL::Renderer;
        this->p_Renderer->Init();
        this->p_Renderer->UpdateView(SCREEN_WIDTH, SCREEN_HEIGHT);
        this->p_Renderer->LoadData();

        this->m_WindowData.renderer = this->p_Renderer;
        this->m_WindowData.scale = &this->m_WindowScale;
        this->m_WindowData.data = this->p_Data;
        this->m_WindowData.animated = false;
        glfwSetWindowUserPointer(this->p_Window, &this->m_WindowData);

        // Load Textures Here //
        
        // ResourceManager::LoadTexture("../../assets/awesomeface.png", "Face", true);
        ResourceManager::LoadTexture("../../assets/dashboardUI.png", "Dashboard", true);
        ResourceManager::LoadTexture("../../assets/dashboardGearShift.png", "GearShift", true);

        ResourceManager::LoadTexture("../../assets/Styled_Dashboard.png", "Styled-Dashboard", true);
        ResourceManager::LoadTexture("../../assets/Styled_Gear-Highlighter.png", "Styled-GearShift", true);
        ResourceManager::LoadTexture("../../assets/Styled_Fuel-Gauge.png", "Styled-FuelGauge", true);
        ResourceManager::LoadTexture("../../assets/Styled_Temp-Gauge.png", "Styled-TempGauge", true);

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
        // if (this->m_WindowData.animated)
        //     Animate();

        // Render UI Here //
        styledBoard();
    }

    void GUIModule::mvpBoard() {
        glm::vec2 screenSize = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) * this->m_WindowScale;

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
                this->p_Renderer->DrawCam(
                    ResourceManager::GetCamera(0), 
                    { 50.0f, 50.0f },
                    { 640.0f, 480.0f }
                );
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
        
        //string version of the shared data values
        std::stringstream str_PI, str_CVT, str_RPM, str_MPH;
        str_PI << this->p_Data->pi_Heat << "%";

        this->p_Renderer->DrawText(
            str_PI.str(),
            ResourceManager::GetFont("ComicNeue"),
            this->m_WindowScale * glm::vec2(140.0f, 50.0f),
            this->m_WindowScale.y * 0.8f,
            {0.82f, 0.106f, 0.106f }
        );
        
        str_CVT << this->p_Data->CVT_Heat << "%";
        
        this->p_Renderer->DrawText(
            str_CVT.str(),
            ResourceManager::GetFont("ComicNeue"),
            this->m_WindowScale * glm::vec2(170.0f, 120.0f),
            this->m_WindowScale.y * 0.8f,
            {0.82f, 0.106f, 0.106f }
        );

        str_MPH << abs(this->p_Data->milesPerHour);

        this->p_Renderer->DrawText(
            str_MPH.str(),
            ResourceManager::GetFont("ComicNeue"),
            this->m_WindowScale * glm::vec2(440.0f, 170.0f),
            this->m_WindowScale.y * 1.5f,
            {0.82f, 0.106f, 0.106f }
        );

        float mph_bar_size = (static_cast<float>(abs(this->p_Data->milesPerHour) % 30) / 30.0f) * 300.0f;

        this->p_Renderer->DrawQuad(
            { 0.0f, 0.0f, 0.0f },
            { 360.0f, 235.0f},
            { mph_bar_size, 65.0f}
        );

        str_RPM << abs(this->p_Data->engineRPM);

        this->p_Renderer->DrawText(
            str_RPM.str(),
            ResourceManager::GetFont("ComicNeue"),
            this->m_WindowScale * glm::vec2(90.0f, 460.0f),
            this->m_WindowScale.y * 0.8f,
            {0.82f, 0.106f, 0.106f }
        );

        float rpm_bar_size = (static_cast<float>(abs(this->p_Data->engineRPM) % 3600) / 3600) * 230;

        this->p_Renderer->DrawQuad(
            { 0.0f, 0.0f, 0.0f },
            { 35.0f, 490.0f},
            { rpm_bar_size, 50.0f}
        );

        float fuel_in_tank_bar = (abs(this->p_Data->fuel) / 2) * -267;

        this->p_Renderer->DrawQuad(
            { 0.0f, 0.0f, 0.0f },
            { 847.0f, 573.0f},
            { 117.0f, fuel_in_tank_bar}
        );
    }

    void GUIModule::styledBoard() {
        glm::vec2 screenSize = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) * this->m_WindowScale;
        glm::vec3 backgroundColor { 1.0f, 1.0f, 1.0f };

        // Gear Shifter
        // 
        // Highlight the active gear, this thankfully is just adjusting the x coordinate
        // of the highlighter to align with the current gear.
        //
        glm::vec2 gearShiftPos = { 863.0f, 479.0f };
        switch (this->p_Data->gearPosition) {
            case PARK:       break;
            case REVERSE:    gearShiftPos.x = 695.0f; break;
            case NEUTRAL:    gearShiftPos.x = 527.0f; break;
            case DRIVE:      gearShiftPos.x = 360.0f; break;
            case ONE:        gearShiftPos.x = 192.0f; break;
            case TWO:        gearShiftPos.x = 24.0f;  break;
        }

        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Styled-GearShift"),
            gearShiftPos, 
            { 135.0f, 98.0f },
            0.0f,
            { 1.0f, 0.0f, 0.0f }
        );

        // Fuel Gauge
        //
        float usedFuelDist = 409.0f - ((this->p_Data->fuel / 7570.824f) * 409.0f);
        float fuelColorStrength = usedFuelDist / 409.0f;

        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Styled-FuelGauge"),
            { 856.0f, 24.0f },
            { 142.5f, 409.0f },
            0.0f,
            { fuelColorStrength, 0.9f - fuelColorStrength, 0.0f }
        );

        this->p_Renderer->DrawQuad(
            backgroundColor,
            { 856.0f, 24.0f },
            { 142.5f, usedFuelDist }
        );

        // MPH
        //
        std::stringstream ss;
        ss << this->p_Data->milesPerHour;

        float mphModifier = 0.0f;
        if (this->p_Data->milesPerHour < 10)
            mphModifier = 40.0f;

        this->p_Renderer->DrawText(
            ss.str(),
            ResourceManager::GetFont("ComicNeue"),
            { 460.0f + mphModifier, 180.0f },
            3.5f,
            { 0.0f, 0.0f, 0.0f }
        );

        // Engine RPM
        //
        float usedRpmDist = 254.0f - ((this->p_Data->engineRPM / 3000.0f) * 254.0f);
        float rpmColorStrength = usedRpmDist / 254.0f;

        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Styled-TempGauge"),
            { 52.0f, 352.0f },
            { 254.0f, 53.0f },
            0.0f,
            { 0.9f - rpmColorStrength, rpmColorStrength, 0.0f }
        );

        this->p_Renderer->DrawQuad(
            backgroundColor,
            { 306.0f, 352.0f },
            { -usedRpmDist, 53.0f }
        );

        // Pi Temp
        // 
        float usedPiHeatDist = 254.0f - ((this->p_Data->pi_Heat / 80.0f) * 254.0f);
        float piHeatColorStrength = usedPiHeatDist / 254.0f;

        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Styled-TempGauge"),
            { 52.0f, 24.0f },
            { 254.0f, 53.0f },
            0.0f,
            { 0.9f - piHeatColorStrength, piHeatColorStrength, 0.0f }
        );

        this->p_Renderer->DrawQuad(
            backgroundColor,
            { 306.0f, 24.0f },
            { -usedPiHeatDist, 53.0f }
        );

        // CVT Temp
        // 
        float usedCvtHeatDist = 254.0f - ((this->p_Data->CVT_Heat / 80.0f) * 254.0f);
        float cvtHeatColorStrength = usedCvtHeatDist / 254.0f;

        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Styled-TempGauge"),
            { 52.0f, 149.0f },
            { 254.0f, 53.0f },
            0.0f,
            { 0.9f - cvtHeatColorStrength, cvtHeatColorStrength, 0.0f }
        );

        this->p_Renderer->DrawQuad(
            backgroundColor,
            { 306.0f, 149.0f },
            { -usedCvtHeatDist, 53.0f }
        );

        // Dashboard Background
        // 
        this->p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Styled-Dashboard"),
            { 0.0f, 0.0f },
            screenSize
        );
    }

    #define MAX_RPM 3300.0f
    #define MAX_MPH 30

    void FluctuateRPM(double dt, int& current, int min = 0, int max = MAX_RPM, int idle = 800) {
        static std::default_random_engine generator;
        static std::uniform_int_distribution<int> idle_distribution(idle, max / 3);
        static std::uniform_int_distribution<int> fluctuation_distribution(-5, 5);
    
        int target_rpm;
    
        // Determine the target RPM based on current RPM
        if (current < idle) {
            target_rpm = idle_distribution(generator);  // If below idle, go to idle or higher
        }
        else {
            target_rpm = std::max(min, std::min(max, current + (fluctuation_distribution(generator) * 10)));
        }
    
        // Simulate smooth transition to target RPM
        current += (target_rpm - current) * dt * 0.05f;  // Gradual change based on delta time
        current = std::max(min, current);  // Clamp to min and max RPM
    }

    void GUIModule::Animate() {
        static Time start = timeNow();
        static Time acStart = timeNow();
        static Time dt;
        static Time at;
        dt = timeFrom(start);
        at = timeFrom(acStart);

        if (timeRealiSec(dt) > 45.0f) {
            start = timeNow();
        }

        // Engine RPM
        //
        if(timeRealiSec(at) > 0.01f){
            FluctuateRPM(timeRealiSec(dt), this->p_Data->engineRPM);
            acStart = timeNow();
        }

        // MPH
        //
        this->p_Data->milesPerHour = (this->p_Data->engineRPM / MAX_RPM) * 30.0f;

        // Fuel
        //
        this->p_Data->fuel -= this->p_Data->milesPerHour * 0.000001f * timeRealiSec(dt);

        if (this->p_Data->fuel <= 0.01f)
            this->p_Data->fuel = 2.0f;

    }

}   // BB