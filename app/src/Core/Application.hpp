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

/** @brief The working namespace for CarBug. */
namespace CB {
    
    /** @brief The core Application class, everything should be controlled or owned by this class. */
    class Application {
    public:
        /** @brief Initializes the Application. */
        void Init();
        /** @brief Shut's down the Application. */
        void Shutdown();

        /** @brief Run's the Application, everything in the runtime will be here. */
        void Run();

        /** @brief Requests to end the runtime. */
        void RequestShutdown();

    private:
        bool m_Running = false; /** @brief Boolean of whether the Application is in runtime or not. */
    };

}   // CB
