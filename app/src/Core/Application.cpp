#include "Application.hpp"

#include "Core/Time.hpp"
#include "Core/SharedData.hpp"

#include "Modules/GUI/GUIModule.hpp"

// std
#include <ctime>
#include <iostream>

namespace BB {

    void Application::Init() {
        std::cout << "Initializing Application!" << std::endl;

        this->p_SharedData = new SharedData;

        // Assuming the modules do no need to run on the main thread,
        // we can run it on a separate thread using the following member.
        // this->m_GUIThread.AddMethod(std::bind(&Application::UpdateModules, this), 1);

        // This is how to add a Module to the Application.
        // The GUI Module Should always be first so that it can be updated separately.
        this->m_Modules.emplace_back(new GUIModule);
        this->m_Modules.back()->Init(this->p_SharedData);

        // Other Modules to be added and initialized below.
    }

    void Application::Shutdown() {
        std::cout << "Shutting Down Application!" << std::endl;

        // This will shut down and delete all modules.
        // Nothing more should be needed here.
        for (size_t i = 0; i < this->m_Modules.size(); i++) {
            this->m_Modules[i]->Shutdown();
            delete this->m_Modules[i];
        }

        delete this->p_SharedData;
    }

    // Runs the application.
    void Application::Run() {
        this->p_SharedData->Running = true;

        Timestep currTime(timeRealiSec(timeNow()));

        Timestep time(0.0f);
        unsigned int frameCount = 0;

        // this->m_GUIThread.Start();

        // This while loop is the runtime. All Module updates will be called from here.
        while (this->p_SharedData->Running) {
            Timestep t(timeRealiSec(timeNow()));
            Timestep frameTime(t - currTime);
            currTime = t;

            time += frameTime;

            if (time.GetSeconds() >= 5.0f) {
                time -= 5.0f;

                std::cout << "Frames per 5 seconds on update thread: " << frameCount << " (" << frameCount / 5.0f << ")" << std::endl;
                frameCount = 0;
            }

            UpdateModules();

            Render();

            frameCount++;
        }

        // this->m_GUIThread.Stop();
    }

    // Forces module_update
    void Application::UpdateModules() {
        for (size_t i = 1; i < this->m_Modules.size(); i++)
            this->m_Modules[i]->Update();
    }

    // Pushes update into GUI
    void Application::Render() {
        // std::cout << timeNow() << " Renderer Thread" << std::endl;

        this->m_Modules[0]->Update();
    }

} // BB
