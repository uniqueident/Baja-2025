#pragma once

namespace BB {

    enum GearPosition {
        PARK = 0,

        REVERSE = 1,
        NEUTRAL = 2,
        DRIVE = 3,
        
        ONE = 4,
        TWO = 5,

    };  // GearPosition

    /** 
     * @brief The only data that the modules can access outside of themselves. This is soley for ease of use in the GUI.
     */
    struct SharedData {
        bool running; /** @brief Boolean of whether the Application is in runtime or not. */

        GearPosition gearPosition; /** @brief The gear position of the transmission as a enum value. */
        int engineRPM; /** @brief The RPM of the engine as a int. */
        int milesPerHour; /** @brief The MPH of the car as a int. */

        SharedData() : running(false), gearPosition(GearPosition::PARK), engineRPM(0), milesPerHour(0) { }

    };  // SharedData

}   // BB