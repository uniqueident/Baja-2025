#include "Application.hpp"

// std
#include <iostream>

namespace CB {

    void Application::Init() {
        std::cout << "Initializing Application!" << std::endl;
    }

    void Application::Shutdown() {
        std::cout << "Shutting Down Application!" << std::endl;
    }

    void Application::Run() {
        m_Running = true;

        // This while loop is the runtime. All Module updates will be called from here.
        while (m_Running) {
            std::cout << "Updated\n";

            RequestShutdown();
        }
    }

    void Application::RequestShutdown() {
        m_Running = false;
    }

} // CB
