#include "GUIModule.hpp"

#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/ResourceManager.hpp"

// std
#include <cstddef>
#include <iostream>
#include <exception>
#include <sstream>

// libs
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CB {

    #define SCREEN_WIDTH 800
    #define SCREEN_HEIGHT 600

    void OpenGLMessageCallback(
        [[gnu::unused]]unsigned source,
        [[gnu::unused]]unsigned type,
        [[gnu::unused]]unsigned id,
        unsigned severity,
        [[gnu::unused]]int lenght,
        const char* message,
        [[gnu::unused]]const void* userParam
    ) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cerr << message << std::endl;
                return;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cerr << message << std::endl;
                return;
            case GL_DEBUG_SEVERITY_LOW:
                std::cerr << message << std::endl;
                return;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cout << message << std::endl;
                return;
        }
    }

    void glfwErrorCallback(int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

    void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {        
        WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        
        data->renderer->UpdateView(width, height);
        data->scale->x = width / static_cast<float>(SCREEN_HEIGHT);
        data->scale->y = height / static_cast<float>(SCREEN_HEIGHT);
    }

    void GUIModule::Init(SharedData * data) {
        p_Data = data;
        m_Closed = false;
        m_WindowScale = { 1.0f, 1.0f };

        if (!glfwInit())
            std::cerr << "Failed to initialize GLFW!" << std::endl;

        glfwSetErrorCallback(glfwErrorCallback);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        p_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Buggie Bug Dashboard", NULL, NULL);
        if (!p_Window) {
            std::cerr << "Failed to create GLFW Window or OpenGL context!" << std::endl;

            glfwTerminate();
            m_Closed = true;

            std::cerr << "GLFW has been closed, please check for issues and run again." << std::endl;
            return;
        }

        glfwMakeContextCurrent(p_Window);
        
        if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD loader!" << std::endl;

            this->Shutdown();
            m_Closed = true;

            std::cerr << "GLFW has been closed, please check for issues and run again." << std::endl;
            return;
        }

        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(p_Window, glfwFramebufferSizeCallback);

        try {
            glDebugMessageCallback(OpenGLMessageCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        }
        catch(std::out_of_range const&) {
            std::cerr << "OpenGL version likely does not support Debug Messaging!" << std::endl;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        p_Renderer = new GL::Renderer;
        p_Renderer->Init();
        p_Renderer->UpdateView(SCREEN_WIDTH, SCREEN_HEIGHT);
        p_Renderer->LoadData();

        m_WindowData.renderer = p_Renderer;
        m_WindowData.scale = &m_WindowScale;
        glfwSetWindowUserPointer(p_Window, &m_WindowData);

        // Load Textures Here //
        
        ResourceManager::LoadTexture("../../assets/awesomeface.png", "Face", true);

        ResourceManager::LoadFont("../../assets/Fonts/ComicNeue-Bold.ttf", "ComicNeue");
    }

    void GUIModule::Shutdown() {
        p_Renderer->Shutdown();
        delete p_Renderer;

        ResourceManager::Clear();

        glfwDestroyWindow(p_Window);

        glfwTerminate();
    }

    void GUIModule::Update() {
        if (glfwWindowShouldClose(p_Window)) {
            p_Data->Running = false;

            return;
        }
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Render();

        glfwSwapBuffers(p_Window);
    }

    void GUIModule::Render() {
        // Render UI Here //

        p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Face"),
            { 100.0f, 100.0f},
            { 200.0f, 300.0f },
            30.0f,
            { 0.0f, 0.0f, 1.0f }
        );

        p_Renderer->DrawQuad(
            { 0.0f, 1.0f, 0.0f },
            { 200.0f, 200.0f},
            { 300.0f, 400.0f },
            45.0f
        );

        std::stringstream ss;
        ss << "Window Scale: " << m_WindowScale.x << "(x), " << m_WindowScale.y << "(y)";

        p_Renderer->DrawText(
            ss.str().c_str(),
            ResourceManager::GetFont("ComicNeue"),
            { 50.0f, 50.f },
            1.0f,
            {0.0f, 0.f, 0.0f }
        );
    }

}   // CB
