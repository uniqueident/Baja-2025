/**
 * @file Application.hpp
 * @author Evan F.
 * @brief The Application header file. Declares the Application class members and methods.
 * @version 1.0
 * @date 10-25-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

// std
#include <vector>

/** @brief The working namespace for CarBug. */
namespace BB {

    struct SharedData;

    class Module;
    
    /** @brief The core Application class, everything should be controlled or owned by this class. */
    class Application {
    public:
        Application() = default;
        ~Application() = default;

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

        SharedData* p_SharedData = nullptr; /** @brief The SharedData for the application. */

        std::vector<Module*> m_Modules = { }; /** @brief The Application Modules that complete tasks. */
    };

}   // BB
