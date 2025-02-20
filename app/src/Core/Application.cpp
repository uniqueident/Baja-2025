#include "Application.hpp"

#include "Core/SharedData.hpp"

#include "Modules/GUI/GUIModule.hpp"

#include "Modules/Sensors/TempProbe.hpp"

// std
#include <ctime>
#include <iostream>

namespace BB {

    Application::Application() :
        p_SharedData(nullptr),
        p_GUI(nullptr),
        m_Modules(0),
        m_Thread()
    {
        this->p_SharedData = new SharedData;

        this->p_GUI = new GUIModule(this->p_SharedData);
    }

    Application::~Application() {
        delete this->p_SharedData;

        delete this->p_GUI;
    }

    void Application::Init() {
        std::cout << "Initializing Application!" << std::endl;

        // ==================== WiringPi Setup ====================
        //

        // Assuming the modules do no need to run on the main thread,
        // we can run it on a separate thread using the following member.
        // this->m_Thread.AddMethod(std::bind(&Application::UpdateModules, this), 1);

        // ==================== Module Setup ====================
        //
        this->m_Modules.emplace_back(new TempProbe(this->p_SharedData))->Init();

    }

    void Application::Shutdown() {
        std::cout << "Shutting Down Application!" << std::endl;

        // This will shut down and delete all modules.
        // Nothing more should be needed here.
        //
        for (size_t i = 0; i < this->m_Modules.size(); i++) {
            this->m_Modules[i]->Shutdown();
            delete this->m_Modules[i];
        }
    }

    // Runs the application.
    //
    void Application::Run() {
        this->p_SharedData->running = true;

        // this->m_Thread.Start();

        // This while loop is the runtime. All Module updates will be called from here.
        //
        // If the separate thread is being used, remove the correlating function from the
        // update loop so that it does not get called twice.
        //
        while (this->p_SharedData->running) {
            UpdateModules();

            Render();
        }

        // this->m_Thread.Stop();
    }

    // Updates all of the Application's modules.
    //
    void Application::UpdateModules() {
        for (size_t i = 0; i < this->m_Modules.size(); i++)
            this->m_Modules[i]->Update();
    }

    // Updates the Application's GUI
    //
    void Application::Render() {
        this->p_GUI->Update();
    }

} // BB
