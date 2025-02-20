#pragma once

#include "Core/ThreadRunner.hpp"

// std
#include <vector>

/** @brief The working namespace for CarBug. */
namespace BB {

    struct SharedData;

    class Module;
    
    /** @brief The core Application class, everything should be controlled or owned by this class. */
    class Application {
    public:
        Application();
        ~Application();

        /** @brief Initializes the Application. */
        void Init();
        /** @brief Shut's down the Application. */
        void Shutdown();

        /** @brief Run's the Application, everything in the runtime will be here. */
        void Run();

    private:
        Application(const Application&) = delete;
        Application operator = (const Application&) = delete;

        // TODO: Make UpdateModules() and Render() static so that they can updated on separate threads. 

        /** @brief Updates all of the Modules for the Application, skips the first Module which should always be the GUI Module. */
        void UpdateModules();

        /** @brief Updates the GUI Module to render. */
        void Render();

        /* --- */

        SharedData* p_SharedData; /** @brief The SharedData for the application. */

        Module* p_GUI; /** @brief The Application GUI module. */
        std::vector<Module*> m_Modules; /** @brief The Application Modules that complete tasks. */

        ThreadRunner m_Thread; /** @brief The separate thread to use for update calls. */
    };

}   // BB
