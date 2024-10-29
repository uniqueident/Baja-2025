/**
 * @file EventHandler.hpp
 * @author Evan F.
 * @brief The EventHandler header file. Declares the EventHandler class members and methods.
 * @version 1.0
 * @date 10-25-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

namespace CB {

    /** @brief Collects and handle's / propagates events. */
    class EventHandler {
    public:
        /** @brief Initializes the EventHandler. */
        void Init();
        /** @brief Shut's down the EventHandler. */
        void Shutdown();

        /** @brief Update's the EventHandler. */
        void Update();
        

    private:

    };

}   // CB
