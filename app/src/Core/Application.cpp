#include "Application.hpp"

#include "ThreadPool.hpp"
#include "SharedData.hpp"

// #include "../Modules/moduletemplate.hpp"
#include "../Modules/GUI/GUIModule.hpp"

// std
#include <iostream>

namespace CB {

    void Application::Init() {
        std::cout << "Initializing Application!" << std::endl;

        p_Pool = new ThreadPool(2);
        p_SharedData = new SharedData;

        // This is how to add a Module to the Application.
        // The GUI Module Should always be first so that it can be updated separately.
        m_Modules.emplace_back(new GUIModule);
        m_Modules.back()->Init(p_SharedData);

        // Other Modules to be added and initialized below.
    }

    void Application::Shutdown() {
        std::cout << "Shutting Down Application!" << std::endl;

        // This will shut down and delete all modules.
        // Nothing more should be needed here.
        for (size_t i = 0; i < m_Modules.size(); i++) {
            m_Modules[i]->Shutdown();
            delete m_Modules[i];
        }

        delete p_SharedData;
        delete p_Pool;
    }
    //runs the application.
    void Application::Run() {
        m_Running = true;
        int i = 0;

        // This while loop is the runtime. All Module updates will be called from here.
        while (m_Running) {
            std::cout << "Updated\n";

            UpdateModules();
            i++;

            Render();

            if (i > 10) // Temporary, fixed update cycle until user interaction enabled.
                RequestShutdown();
        }
    }
    // Calls application shutdown.
    void Application::RequestShutdown() {
        m_Running = false;
    }

    // Forces module_update
    void Application::UpdateModules() {
        for (size_t i = 1; i < m_Modules.size(); i++)
            m_Modules[i]->Update();
    }

    // Pushes update into GUI
    void Application::Render() {
        m_Modules[0]->Update();
    }

} // CB
