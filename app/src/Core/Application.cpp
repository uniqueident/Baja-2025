#include "Application.hpp"

#include "Core/SharedData.hpp"

#include "Modules/GUI/GUIModule.hpp"

// std
#include <iostream>

namespace CB {

    void Application::Init() {
        std::cout << "Initializing Application!" << std::endl;

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
    }
    //runs the application.
    void Application::Run() {
        p_SharedData->Running = true;

        // This while loop is the runtime. All Module updates will be called from here.
        while (p_SharedData->Running) {
            UpdateModules();

            Render();
        }
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
