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
        /**
         * @brief Boolean of whether the Application is in runtime or not.
         */
        bool running;

        /**
         * @brief The gear position of the transmission as a enum value.
         */
        GearPosition gearPosition;
        /**
         * @brief The RPM of the engine as a int.
         */
        int engineRPM;
        /**
         * @brief The MPH of the car as a int.
         */
        int milesPerHour;
        /**
         * @brief The heat value of the Continuously Variable Transmission.
         */
        float CVT_Heat;
        /**
         * @brief The heat value of the pi.
         */
        float pi_Heat;
        /**
         * @brief The data representation of the amount of fuel in the tank 
         */
        float fuel;

        SharedData() : running(false), gearPosition(GearPosition::PARK), engineRPM(0), milesPerHour(0), CVT_Heat(0.0), pi_Heat(0.0), fuel(0.0) { }

    };  // SharedData

}   // BB