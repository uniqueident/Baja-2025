#include "TempProbe.hpp"

#include "Core/SharedData.hpp"

// libs
#ifdef RPI_PI

    #include <wiringPi.h>
    #include <wiringPiSPI.h>

#endif

namespace BB {

    TempProbe::TempProbe(SharedData* data, Physical clock, Physical miso, Physical mosi, Physical ce0) :
        Module(data),
        m_ClockPin(clock),
        m_MisoPin(miso),
        m_MosiPin(mosi),
        m_Ce0Pin(ce0),
        m_fd(0)
    {
        this->p_Data->RegisterPin(m_ClockPin);
        this->p_Data->RegisterPin(m_MisoPin);
        this->p_Data->RegisterPin(m_MosiPin);
        this->p_Data->RegisterPin(m_Ce0Pin);
    }

    TempProbe::~TempProbe() {
        this->p_Data->UnregisterPin(m_ClockPin);
        this->p_Data->UnregisterPin(m_MisoPin);
        this->p_Data->UnregisterPin(m_MosiPin);
        this->p_Data->UnregisterPin(m_Ce0Pin);
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

    void TempProbe::Update() {
        // Reset the buffer for a new data request.
        //
        this->m_Buffer[0] = 1;
        this->m_Buffer[1] = static_cast<unsigned char>((8 + this->k_Channel) << 4);
        this->m_Buffer[2] = 0;

        // ==================== WiringPi Actions ====================
        //
    #ifdef RPI_PI

        unsigned short retVal = 0;

        wiringPiSPIDataRW(this->k_Channel, this->m_Buffer, 3);

        digitalWrite(this->m_Ce0Pin, HIGH);

        delay(10);

        retVal = (((this->m_Buffer[1] & 0x03) << 8) | this->m_Buffer[2]) & 0x3FF;

        float voltage = retVal * (5.0 / 1023.0);

    #endif
    }

}   // BB
