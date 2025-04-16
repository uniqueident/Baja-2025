#include "TempProbe.hpp"

#include "Core/SharedData.hpp"

#include <cmath>

// libs
#ifdef RPI_PI

    #include <wiringPi.h>
    #include <wiringPiSPI.h>
    #include <max31855.h>

    #include <byteswap.h>

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

        uint8_t data[2];
        data[0] = 0x08; // Write Signal
        // Bit | Purpose                  | Value
        // --------------------------------------
        // 7   | Vbias                    | 1
        // 6   | Conversion Mode (Auto)   | 1
        // 5   | 1-Shot (Ignored in Auto) | 0
        // 4   | 3-Wire                   | 1
        // 3   | Fault Detection Cycle    | 0
        // 2-1 | Fault Detection Bits     | 00
        // 1   | Fault Status Clear       | 0
        //
        data[1] = 0b11010010;

        wiringPiSPIDataRW(this->k_Channel, data, 2);

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
        // ==================== SPI Data Setup ====================
        //
        uint8_t tx[3];
        tx[0] = 0x01 & 0x7F;
        tx[1] = 0;
        tx[2] = 0;

        // ==================== WiringPi Actions ====================
        //
    #ifdef RPI_PI

        wiringPiSPIDataRW(this->k_Channel, tx, 3);
        uint8_t msb = tx[1]; // Most significant bits
        uint8_t lsb = tx[2]; // Least significant bits

        uint16_t data = ((msb << 8) | lsb) >> 1; // Remove fault bit

        float resistance = ((float)data * 430.0f) / 32768.0f;
        float temp = 0.0f;

        printf("RTD: %u | Resistance: %.2f ohm | Temp: %.2f C\n", data, resistance, temp);

        delay(1000);

    #endif
    }

}   // BB
