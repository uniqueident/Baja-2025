#include "TempProbe.hpp"

#include <iostream>

#include "Core/SharedData.hpp"

// libs
#ifdef RPI_PI

    #include <wiringPi.h>
    #include <wiringPiSPI.h>

#endif

namespace BB {

    TempProbe::TempProbe(SharedData* data, Physical clock, Physical miso, Physical mosi, Physical ce0) : 
        Module(data)
    {
        
    }

    TempProbe::~TempProbe() {
        // ==================== WiringPi Pin Cleanup ====================
        //

    }

    void TempProbe::Init() {
        // ==================== WiringPi Pin Setup ====================
        //
    #ifdef RPI_PI

        // Pins to MCP3008 (Temp Probe Interpreter)
        // 
        // MCP3008 VDD to Raspberry Pi 3.3V       (Pin 1)
        // MCP3008 VREF to Raspberry Pi 3.3V      (Pin 17)
        // MCP3008 AGND to Raspberry Pi GND       (Pin 20)
        // MCP3008 DGND to Raspberry Pi GND       (Pin 25)
        // MCP3008 CLK to Raspberry Pi SCLK       (Pin 23)
        // MCP3008 DOUT to Raspberry Pi MISO      (Pin 21)
        // MCP3008 DIN to Raspberry Pi MOSI       (Pin 19)
        // MCP3008 CS/SHDN to Raspberry Pi CE0    (Pin 24)

        wiringPiSPISetup(this->k_Channel, this->k_mhz);

        wiringPiSPIDataRW(this->k_Channel, this->m_Buffer, 3);
        //pinMode(Physical::PIN_21, INPUT);
        //pinMode(Physical::PIN_23,OUTPUT);

    #endif
    }

    void TempProbe::Shutdown() {
        
    }

    void TempProbe::Update() {
        // ==================== WiringPi Actions ====================
        //
    #ifdef RPI_PI



        if (digitalRead(Physical::PIN_21) == HIGH){
            std::cout << "HIGH Signal" << std::endl;

            this->p_Data->pi_Heat = 5;
        }
        ++i;

    #endif
    }

}   // BB
