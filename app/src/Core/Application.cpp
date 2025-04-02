#include "Application.hpp"

#include "Core/SharedData.hpp"

#include "Modules/GPIO/FanControl.hpp"
#include "Modules/GPIO/FuelFlow.hpp"
#include "Modules/GPIO/TempProbe.hpp"

#include "Modules/GUI/GUIModule.hpp"


// std
#include <cstdlib>
#include <ctime>
#include <iostream>

// libs
#ifdef RPI_PI

    #include <wiringPi.h>

#endif

namespace BB {

    Application::Application() :
        p_SharedData(nullptr),
        p_GUI(nullptr),
        m_Modules(0),
        m_Thread()
    {
        this->p_SharedData = new SharedData;

        this->p_GUI = new GUIModule(this->p_SharedData);

        srand(time(0));
    }

    Application::~Application() {
        delete this->p_SharedData;

        delete this->p_GUI;
    }

    void Application::Init() {
        std::cout << "Initializing Application!" << std::endl;

        // this->p_GUI->Init();

        // ==================== WiringPi Setup ====================
        //
    #ifdef RPI_PI

        wiringPiSetupPinType(WPI_PIN_PHYS);

    #endif

        // Assuming the modules do no need to run on the main thread,
        // we can run it on a separate thread using the following member.
        //
        this->m_Thread.AddMethod(std::bind(&Application::UpdateModules, this), 1);

        // ==================== Module Setup ====================
        //
        this->m_Modules.emplace_back(new TempProbe(
            this->p_SharedData,
            Physical::PIN_23,
            Physical::PIN_21,
            Physical::PIN_19,
            Physical::PIN_24
        ))->Init();

        this->m_Modules.emplace_back(new FuelFlow(
            this->p_SharedData,
            Physical::PIN_12
        ))->Init();

        // TODO: Check the fan with for sure 12v power supply to confirm that the GPIO can output a strong enough PWM signal.
        //
        // this->m_Modules.emplace_back(new FanControl(
        //     this->p_SharedData,
        //     Physical::PIN_32,
        //     Physical::PIN_36
        // ))->Init();

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

        // this->p_GUI->Shutdown();
    }

    // Runs the application.
    //
    void Application::Run() {
        this->p_SharedData->running = true;

        this->m_Thread.Start();

        // This while loop is the runtime. All Module updates will be called from here.
        //
        // If the separate thread is being used, remove the correlating function from the
        // update loop so that it does not get called twice.
        //
        while (this->p_SharedData->running) {
            // UpdateModules();

            Render();
        }

        this->m_Thread.Stop();
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
        // this->p_GUI->Update();
    }

} // BB
