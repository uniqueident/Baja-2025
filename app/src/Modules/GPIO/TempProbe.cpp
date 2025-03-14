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
        m_Ce0Pin(ce0)
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

        if (wiringPiSPISetup(this->k_Channel, this->k_hz) == -1)
            throw std::runtime_error("Failed to setup Temp Probe SPI bus!");

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

        unsigned short retVal = 0;

        // ==================== WiringPi Actions ====================
        //
    #ifdef RPI_PI

        wiringPiSPIDataRW(1, this->m_Buffer, 3);

    #endif

        retVal = (this->m_Buffer[1] << 8) | (this->m_Buffer[2]);

        std::cout << "Temp Probe returned Data: " << retVal << std::endl;
    }

}   // BB
