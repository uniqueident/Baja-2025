#include "TempProbe.hpp"

#include "Core/SharedData.hpp"
#include <cmath>

// libs
#ifdef RPI_PI

    #include <wiringPi.h>
    #include <wiringPiSPI.h>

#endif

namespace BB {

    TempProbe::TempProbe(SharedData* data, Physical clock, Physical miso, Physical mosi, Physical ce0, Physical gpio) :
        Module(data),
        m_ClockPin(clock),
        m_MisoPin(miso),
        m_MosiPin(mosi),
        m_Ce0Pin(ce0),
        m_GpioPin(gpio),
        m_fd(0),
        voltage()
    {
        this->p_Data->RegisterPin(m_ClockPin);
        this->p_Data->RegisterPin(m_MisoPin);
        this->p_Data->RegisterPin(m_MosiPin);
        this->p_Data->RegisterPin(m_Ce0Pin);
        this->p_Data->RegisterPin(m_GpioPin);
    }

    TempProbe::~TempProbe() {
        this->p_Data->UnregisterPin(m_ClockPin);
        this->p_Data->UnregisterPin(m_MisoPin);
        this->p_Data->UnregisterPin(m_MosiPin);
        this->p_Data->UnregisterPin(m_Ce0Pin);
        this->p_Data->UnregisterPin(m_GpioPin);
    }

    void TempProbe::Init() {
        // ==================== WiringPi Pin Setup ====================
        // 
    #ifdef RPI_PI

        if ((this->m_fd = wiringPiSPISetup(this->k_Channel, this->k_hz)) == -1)
            throw std::runtime_error("Failed to setup Temp Probe SPI bus!");

        pinMode(this->m_Ce0Pin, OUTPUT);

    #endif
    }

    void TempProbe::Shutdown() {
        // ==================== WiringPi Pin Cleanup ====================
        //
    #ifdef RPI_PI

        wiringPiSPIClose(this->k_Channel);

    #endif
    }

    // Constants for PT100
    const float R0 = 100.0;         // Resistance at 0°C (100 ohms)
    const float alpha = 0.00385;    // Temperature coefficient of resistance (per °C)
    const float vIn = 5.0f;         // The input voltage of the PT100
    const float Rfixed = 23.0f;   // Reference resistor in the circuit

    void TempProbe::Update() {
        // Reset the buffer for a new data request.
        

        //config.
        this->m_Buffer[0] = 0x08;
        //config for 3 wire
        this->m_Buffer[1] = 0xD0;//static_cast<unsigned char>((8 + this->k_Channel) << 4);
        //reuse.
        this->m_Buffer[2] = 0x00;

        // ==================== WiringPi Actions ====================
        //
    #ifdef RPI_PI

        unsigned int retVal = 0;
        //manually set the value.

        wiringPiSPIDataRW(this->k_Channel, this->m_Buffer, 3);
        digitalWrite(this->m_Ce0Pin, HIGH);
        delay(100);
        retVal = (((this->m_Buffer[1]) << 8) | this->m_Buffer[2]) >>1;
        
        // RTD PT100 has 100 ohm resistance at 0 C, so the expected voltage reading will be ~ 4.9V
        //
        //this->voltage = retVal / 1023.0f * 5.0f;
        std::cout<<"RETURNED VALUE: "<<retVal<<std::endl;

    #endif
    }

}   // BB
