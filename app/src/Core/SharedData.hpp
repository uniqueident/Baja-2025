/**
 * @file SharedData.hpp
 * @author Evan F., Mateo M.
 * @brief The SharedData header file. Declares the SharedData struct and any necessary helper functions.
 * @version 1.0
 * @date 10-25-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

namespace BB {

    // TODO: Figure out what is going to be needed here for the GUI.

    /** 
     * @brief The only data that the modules can access outside of themselves. This is soley for ease of use in the GUI.
     */
    struct SharedData {
       bool Running; /** @brief Boolean of whether the Application is in runtime or not. */

        SharedData() : Running(false) { }

    };  // SharedData

}   // BB