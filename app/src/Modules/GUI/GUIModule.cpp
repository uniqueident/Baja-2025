#include "GUIModule.hpp"

#include "Modules/GUI/Renderer/Renderer.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <cstddef>
#include <iostream>

namespace CB {

    void glfwErrorCallback(int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

    void glfwFramebufferSizeCallback([[gnu::unused]]GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void GUIModule::Init(SharedData * data) {
        p_Data = data;
        m_Closed = false;

        if (!glfwInit())
            std::cerr << "Failed to initialize GLFW!" << std::endl;

        glfwSetErrorCallback(glfwErrorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        p_Window = glfwCreateWindow(640, 480, "Buggie Bug Dashboard", NULL, NULL);
        if (!p_Window) {
            std::cerr << "Failed to create GLFW Window or OpenGL context!" << std::endl;

            glfwTerminate();
            m_Closed = true;

            std::cerr << "GLFW has been closed, please check for issues and run again." << std::endl;
            return;
        }

        glfwMakeContextCurrent(p_Window);
        
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD loader!" << std::endl;

            this->Shutdown();
            m_Closed = true;

            std::cerr << "GLFW has been closed, please check for issues and run again." << std::endl;
            return;
        }

        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(p_Window, glfwFramebufferSizeCallback);

        p_Renderer = new GL::Renderer;
        p_Renderer->Init();
    }

    void GUIModule::Shutdown() {
        p_Renderer->Shutdown();

        if (p_Window)
            glfwDestroyWindow(p_Window);

        glfwTerminate();
    }

    void GUIModule::Update() {
        if (m_Closed)
            return;

        if (glfwWindowShouldClose(p_Window)) {
            glfwDestroyWindow(p_Window);

            m_Closed = true;
            return;
        }

        p_Renderer->Render();

        glfwSwapBuffers(p_Window);
        glfwPollEvents();
    }

}   // CB
