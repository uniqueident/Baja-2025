#pragma once

#include "Core/SharedData.hpp"
#include "Modules/GPIO/Pins.hpp"


#include <iostream>
#include <set>
#include <vector>

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
         * @brief The heat value of the Continuously Variable Transmission, in Celcius.
         */
        float CVT_Heat;
        /**
         * @brief The heat value of the pi, in Celcius.
         */
        float pi_Heat;
        /**
         * @brief The data representation of the amount of fuel in the tank in mL.
         */
        float fuel;

        /**
         * @brief The full tank amount in milliliters. A 2 gallon tank is full with 7570.824 mL.
         */
        #define FUEL_FULL_ML 7570.824f
        /**
         * @brief Resets the fuel level to full.
         */
        void ResetFuel() { this->fuel = FUEL_FULL_ML; }

        /**
         * @brief Build the set of used pins.
         */
        std::set<Physical> usedpins;

        SharedData() :
            running(false),
            gearPosition(GearPosition::PARK),
            engineRPM(0),
            milesPerHour(0),
            CVT_Heat(0.0f),
            pi_Heat(0.0f),
            fuel(FUEL_FULL_ML),
            usedpins()
        { }

        void RegisterPin(Physical pin) {
            if (this->usedpins.find(pin) != this->usedpins.end()) {
                std::cerr << "Pin #" << static_cast<int>(pin) << " is already in use!" << std::endl;

                std::exit(3);
            }

            std::cout << "Pin #" << static_cast<int>(pin) << " is now in use." << std::endl;

            this->usedpins.emplace(pin);
        }

        void UnregisterPin(Physical pin) {
            if (this->usedpins.find(pin) == this->usedpins.end()) {
                std::cerr << "Pin #" << static_cast<int>(pin) << " has not been registered yet!" << std::endl;
            }

            std::cout << "Pin #" << static_cast<int>(pin) << " is now free." << std::endl;

            this->usedpins.erase(pin);
        }

        //visualizes 
        void visualize(){

        }
    };  // SharedData

}   // BB