#pragma once

#include <bits/types/FILE.h>
#include <cstdint>
#include <gpiod.h>
#include <gpiod.hpp>
#include <map>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <set>
#include <sstream>

namespace IO {


        //The state of the detected edge.
        enum Edgestate{
        RISING =0,
        FALLING =1,
        BOTH = 2
        };

        enum GPIO_Pins {
        /**
         * @brief A general use pin.
         * @detail Used for motor control, led control, etc... This can be done in software on all pins.(Except pins 27 and 28) Just for distinctness.
         */
        GEN_PIN_7  = 4,
        GEN_PIN_11 = 17,
        GEN_PIN_13 = 27,
        GEN_PIN_15 = 22,
        GEN_PIN_16 = 23,
        GEN_PIN_18 = 24,
        GEN_PIN_22 = 25,
        GEN_PIN_29 = 5,
        GEN_PIN_31 = 6,
        GEN_PIN_32 = 12,
        GEN_PIN_33 = 13,
        GEN_PIN_36 = 16,
        GEN_PIN_37 = 26,

        /**
         * @brief External device communication pins.
         * @detail I2C standard 2 wire communction with external devices.
         */
        EXT_PIN_3 = 2,
        EXT_PIN_5 = 3,
        /**
         * @brief EEPROM communication pins.
         * @detail I2C standard communication with EEPROM for integrated devices.
         */
        EEP_PIN_27 = 0,
        EEP_PIN_28 = 1,

        /**
         * @brief Async serial communication pins.
         * @detail 
         */
        SER_PIN_8  = 14,
        SER_PIN_10 = 15,

        /**
         * @brief Serial bus communication pins.
         * @detail
         */
        SPI_PIN_19 = 10,
        SPI_PIN_21 = 9,
        SPI_PIN_23 = 11,
        SPI_PIN_24 = 8,
        SPI_PIN_26 = 7,

        /**
         * @brief Audio pulse control pins.
         * @detail Used for all pulse things, primarily frame sync and clock control for external audio devices.
         */
        PLS_PIN_12 = 18,
        PLS_PIN_35 = 19,

        /**
         * @brief Digital audio communication pins.
         * @detail Pin 38 is PCM data in, such as for a mic.
         * @detail Pin 40 is data output signal audio.
         */
        DIG_PIN_38 = 20,
        DIG_PIN_40 = 21,
    };

    inline const char* to_string(GPIO_Pins pin, const char* name){
        if(name !=nullptr ){
            std::cout<<"Pointer is not null, returning pointer"<<std::endl;
            return name;
        }
        std::stringstream find_better_way_to_do_this;
        find_better_way_to_do_this<<pin;
        name = find_better_way_to_do_this.str().c_str();
        return name;
    }

    inline std::ostream &operator <<(std::ostream& outstream, GPIO_Pins pin){
        switch (pin) {
            case GEN_PIN_7  : 
            case GEN_PIN_11 : 
            case GEN_PIN_13 :
            case GEN_PIN_15 : 
            case GEN_PIN_16 : 
            case GEN_PIN_18 : 
            case GEN_PIN_22 :
            case GEN_PIN_29 :
            case GEN_PIN_31 : 
            case GEN_PIN_32 : 
            case GEN_PIN_33 : 
            case GEN_PIN_36 : 
            case GEN_PIN_37 :
            outstream<<"General Purpose Pin";
            break;
            case EXT_PIN_3 : case EXT_PIN_5:
            outstream<<"External Communication Pin";
            break; 
            case EEP_PIN_27 : case EEP_PIN_28 : 
            outstream<<"EEPROM Communication Pin";
            break;
            case SER_PIN_8: case SER_PIN_10 : 
            outstream<<"Async Serial Communication Pin";
            break;
            case SPI_PIN_19 : case SPI_PIN_21: case SPI_PIN_23 : case SPI_PIN_24 : case SPI_PIN_26: 
            outstream<<"Serial Bus Communication Pin";
            break;
            case PLS_PIN_12 : case PLS_PIN_35 :
            outstream<<"Audio Pulse Control Pin";
            break; 
            case DIG_PIN_38: case DIG_PIN_40 : 
            outstream<<"Digital Audio Communication Pin";
            break;
            default:
            outstream<<"Non-Programmable or Invalid Pin";
            break;
        }
        return outstream;
    }

}   // GPIO

struct Pins {

    const std::set<IO::GPIO_Pins> pinset {
        IO::GEN_PIN_7,  IO::GEN_PIN_11, 
        IO::GEN_PIN_13, IO::GEN_PIN_15,
        IO::GEN_PIN_16, IO::GEN_PIN_18,
        IO::GEN_PIN_22, IO::GEN_PIN_29,
        IO::GEN_PIN_31, IO::GEN_PIN_32, 
        IO::GEN_PIN_33, IO::GEN_PIN_36, 
        IO::GEN_PIN_37,
        //
        IO::EXT_PIN_3,  IO::EXT_PIN_5,
        //
        IO::EEP_PIN_27, IO::EEP_PIN_28,
        //
        IO::SER_PIN_8,  IO::SER_PIN_10,
        //
        IO::SPI_PIN_19, IO::SPI_PIN_21, 
        IO::SPI_PIN_23, IO::SPI_PIN_24, 
        IO::SPI_PIN_26,
        //
        IO::PLS_PIN_35, IO::PLS_PIN_12,
        //
        IO::DIG_PIN_38, IO::DIG_PIN_40
    };

    const std::set<IO::GPIO_Pins> generic {
        IO::GEN_PIN_7, IO::GEN_PIN_11, IO::GEN_PIN_13, IO::GEN_PIN_15, IO::GEN_PIN_16, IO::GEN_PIN_18, IO::GEN_PIN_22, IO::GEN_PIN_29, IO::GEN_PIN_31, IO::GEN_PIN_32, IO::GEN_PIN_33, IO::GEN_PIN_36, IO::GEN_PIN_37
    };



    static bool is_Pin(IO::GPIO_Pins pin);

    static bool is_Generic(IO::GPIO_Pins pin);

    static Pins& instance();

private:
    static inline Pins* _instance = nullptr;

};  // Pins

//pointer to struct
class phyisical_to_virtual{
    public:
        
        phyisical_to_virtual(std::string device = "gpiochip0");

        ~phyisical_to_virtual();
        //uses the physical placement of the pins on the board.
        bool request_pin(IO::GPIO_Pins pin);
        void release_pin(IO::GPIO_Pins pin);
        bool set_as_input(IO::GPIO_Pins pin, std::string name);
        bool set_as_output(IO::GPIO_Pins pin,std::string name,int basevalue);
        bool set_as_event(IO::GPIO_Pins pin, IO::Edgestate state, std::string name);

        //sets the value if it worky it say good :)
        bool set_value(IO::GPIO_Pins pin, int value);
        int get_value(IO::GPIO_Pins pin);


        std::map<IO::GPIO_Pins,gpiod_line*> gpio_to_line;
        std::map<IO::GPIO_Pins,gpiod_line_event*> gpio_to_event;
    private:

        struct gpiod_chip* chip;
        phyisical_to_virtual(const phyisical_to_virtual&) = delete;
        phyisical_to_virtual& operator= (const phyisical_to_virtual&) = delete;
        /*
        foo(struct* struct)

        struct f;
        foo(&f)
        */
        //example type each line should be done individually
        //gpiod::line line;
        


};