#include "GUIModule.hpp"

#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/Shader.hpp"
#include "Modules/GUI/Renderer/ResourceManager.hpp"
#include <exception>

// libs
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// std
#include <cstddef>
#include <iostream>

namespace CB {

    #define SCREEN_WIDTH 800
    #define SCREEN_HEIGHT 600

    void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int lenght, const char* message, const void* userParam) {
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
        static_cast<WindowData*>(glfwGetWindowUserPointer(window))->renderer->UpdateView(width, height);
    }

    void GUIModule::Init(SharedData * data) {
        p_Data = data;
        m_Closed = false;

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
        catch(const std::exception e) {
            std::cerr << "OpenGL version likely does not support Debug Messaging!" << std::endl;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ResourceManager::LoadShader("../../assets/Shaders/Sprite.glsl", "Sprite");

        p_Renderer = new GL::Renderer;
        p_Renderer->Init(&ResourceManager::GetShader("Sprite"));

        ResourceManager::GetShader("Sprite").Use();
        ResourceManager::GetShader("Sprite").SetInt("image", 0);

        p_Renderer->UpdateView(SCREEN_WIDTH, SCREEN_HEIGHT);

        p_Renderer->LoadData();

        m_WindowData.renderer = p_Renderer;
        glfwSetWindowUserPointer(p_Window, &m_WindowData);

        ResourceManager::LoadTexture("../../assets/awesomeface.png", "Face", true);
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

    // Render Calls For UI (Draw Sprites)
    void GUIModule::Render() {
        p_Renderer->DrawSprite(
            ResourceManager::GetTexture("Face"),
            { 200.0f, 200.0f},
            { 300.0f, 400.0f },
            { 0.0f, 1.0f, 0.0f },
            45.0f
        );
    }

}   // CB
