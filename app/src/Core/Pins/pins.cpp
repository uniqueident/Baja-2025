#include "pins.hpp"
#include <utility>
#include <bits/types/struct_timespec.h>
#include <ctime>
#include <stdexcept>
#include <system_error>
#include <cstdint>
#include <gpiod.h>
#include <gpiod.hpp>
#include <set>

// std
#include <iostream>
//TODO fix the inputs for both is pin and is generic in order to actually get the pins. as of currently i ma checking whether the GPIO exists, this is incorrect.

#define CONSOLE_TEXT_RED(Text)                      ("\x1b[31m" Text "\033[0m")
#define CONSOLE_TEXT_GREEN(Text)                    ("\x1b[32m" Text "\033[0m")
#define CONSOLE_TEXT_YELLOW(Text)                   ("\x1b[33m" Text "\033[0m")
#define CONSOLE_TEXT_BLUE(Text)                     ("\x1b[34m" Text "\033[0m")
#define CONSOLE_TEXT_MAGENTA(Text)                  ("\x1b[35m" Text "\033[0m")
#define CONSOLE_TEXT_CYAN(Text)                     ("\x1b[36m" Text "\033[0m")
#define CONSOLE_TEXT_WHITE(Text)                    ("\x1b[37m" Text "\033[0m")

//returns whether the pins exist that were given.
bool Pins::is_Pin(IO::GPIO_Pins pin){
    return static_cast<bool>(Pins::instance().pinset.count(pin));
}

//returns if generic or not.
bool Pins::is_Generic(IO::GPIO_Pins pin){
    return static_cast<bool>(Pins::instance().generic.count(pin));
}

//keeps it single.
Pins& Pins::instance(){
    //checks if instance exists
    if (_instance ==nullptr)
        _instance = new Pins;
    //exists return current instance.
    return *_instance;
}

//construct
phyisical_to_virtual::phyisical_to_virtual(std::string device){
    this->chip = gpiod_chip_open_by_name(device.c_str());
    if(!this->chip){
        throw std::runtime_error(CONSOLE_TEXT_RED("Chip access failure."));
    }
}

phyisical_to_virtual::~phyisical_to_virtual(){
    for(const auto &[pin,line]: gpio_to_line){
        std::cout<< "Cleaning up pin: "<<pin<<std::endl;
        gpiod_line_release(gpio_to_line[pin]);
        
    }
    for(const auto &[pin,event]: gpio_to_event){
        std::cout<< "Cleaning up pin: "<<pin<<std::endl;
        delete gpio_to_event[pin];
    }

    this->gpio_to_line.clear();
    gpiod_chip_close(this->chip);

    std::cout<<"Done"<<std::endl;
}

/**
    @param pin The phyical pin in use

    @param consumername A null pointer to be assigned a name
 */
bool phyisical_to_virtual::set_as_input(IO::GPIO_Pins pin, std::string consumername){
    return !gpiod_line_request_input(this->gpio_to_line[pin], IO::to_string(pin,consumername.c_str()));
}
/**
 * @brief 
 * 
 * @param pin 
 * @param consumername 
 * @return true 
 * @return false 
 */

bool phyisical_to_virtual::set_as_output(IO::GPIO_Pins pin,std::string name,int basevalue){
    return !gpiod_line_request_output(this->gpio_to_line[pin],name.c_str(),basevalue);
}

bool phyisical_to_virtual::set_as_event(IO::GPIO_Pins pin, IO::Edgestate state, std::string name){
    gpiod_line* temp = this->gpio_to_line[pin];
    

    switch(state){
        case IO::Edgestate::RISING:
            return !gpiod_line_request_rising_edge_events(temp, name.c_str());
            break;
        case IO::Edgestate::FALLING:
            return !gpiod_line_request_falling_edge_events(temp, name.c_str());
            break;
        case IO::Edgestate::BOTH:
            return !gpiod_line_request_both_edges_events(temp, name.c_str());
            break;
        default: 
        return false;
    }


    return true;
}

/**
 * @brief returns an nullptr if failure or non controllable
 * 
 * @param pin 
 * @return struct gpiod_line* 
 */
 //TODO: FIX THIS AND ADD BIAS.
bool phyisical_to_virtual::request_pin(IO::GPIO_Pins pin){
    //need to convert pin to the gpio stuff
    if(!Pins::is_Pin(pin) || this->gpio_to_line.count(pin) ==1){
        std::cerr << "Requested Invalid or Non-controllable Pin!" << std::endl;
        return false;
    }
        std::cout<<CONSOLE_TEXT_WHITE("Selected: ")<<pin<<std::endl;
        gpio_to_line[pin] = gpiod_chip_get_line(this->chip,pin);

        //needed  in case of failure.
        if(!gpio_to_line[pin]){
            gpio_to_line.erase(pin);
            std::cout<<"getting line failed"<<std::endl;
            return false;
        }

        return true;
}
/**
 * @brief 
 * 
 * @param pin 
 */
void phyisical_to_virtual::release_pin(IO::GPIO_Pins pin){
    if(!Pins::is_Pin(pin)|| this->gpio_to_line.count(pin) ==0){
        std::cerr << "Requested Invalid or Non-controllable Pin!" << std::endl;
        return;
    }
        std::cout<<CONSOLE_TEXT_WHITE("Releasing: ")<<pin<<std::endl;
        gpiod_line_release(this->gpio_to_line[pin]);
        this->gpio_to_line.erase(pin);
        return;
}


bool phyisical_to_virtual::set_value(IO::GPIO_Pins pin, int state){
    return (gpiod_line_set_value(gpio_to_line[pin],state)==0)? true:false;
}

//Gets the value.
int phyisical_to_virtual::get_value(IO::GPIO_Pins pin){
    int value = gpiod_line_get_value(gpio_to_line[pin]);
    
    return (value>-1)? value: -1;
}