#include "Application.hpp"

#include "ThreadPool.hpp"

// std
#include <iostream>

namespace CB {

    void Application::Init() {
        std::cout << "Initializing Application!" << std::endl;

        m_Pool = new ThreadPool(2);
    }

    void Application::Shutdown() {
        std::cout << "Shutting Down Application!" << std::endl;

        delete m_Pool;
    }

    void TaskMessage(int i) { std::cout << "Message " << i << " From Thread " << std::this_thread::get_id() << std::endl; }

    void Application::Run() {
        m_Running = true;
        int i = 0;

        // This while loop is the runtime. All Module updates will be called from here.
        while (m_Running) {
            std::cout << "Updated\n";

            m_Pool->AddTask(TaskMessage, (i));
            i++;

            if (i > 10)
                RequestShutdown();
        }
    }

    void Application::RequestShutdown() {
        m_Running = false;
    }

} // CB
