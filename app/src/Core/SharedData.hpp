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