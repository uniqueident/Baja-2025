#include "TempProbe.hpp"

#include "Core/SharedData.hpp"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>

// libs
#ifdef RPI_PI

    #include <wiringPi.h>
    #include <wiringPiSPI.h>

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

    #define CONFIG_REG            0x00
    #define CONFIG_BIAS           0x80
    #define CONFIG_MODE_AUTO      0x40
    #define CONFIG_MODEOFF        0x00
    #define CONFIG_1_SHOT         0x20
    #define CONFIG_3_WIRE         0x10
    #define CONFIG_2_4_WIRE       0x00
    #define CONFIG_FAULT_STATUS   0x02
    #define CONFIG_FILTER_50HZ    0x01
    #define CONFIG_FILTER_60HZ    0x00

    #define RTD_MSB_REG           0x01
    #define RTD_LSB_REG           0x02
    #define HFAULT_MSB_REG        0x03
    #define HFAULT_LSB_REG        0x04
    #define LFAULT_MSB_REG        0x05
    #define LFAULT_LSB_REG        0x06
    #define FAULT_STATUS_REG      0x07

    #define FAULT_HIGH_THRESHOLD   0x80
    #define FAULT_LOW_THRESHOLD    0x40
    #define FAULT_REF_IN_LOW       0x20
    #define FAULT_REF_IN_HIGH      0x10
    #define FAULT_RTD_IN_LOW       0x08
    #define FAULT_OV_UV            0x04
    #define FAULT_NONE             0x00

    #define WIRE_COUNT             3
    #define FILTER_HZ              50

    void TempProbe::ReadRegisterN(uint8_t address, uint8_t* buffer, uint8_t n) {
        address &= 0x7F;

    #ifdef RPI_PI

        digitalWrite(this->m_Ce0Pin, LOW);

        wiringPiSPIDataRW(this->k_Channel, static_cast<char*>(&address), n);

        digitalWrite(this->m_Ce0Pin, HIGH);

    #endif
    }

    uint8_t TempProbe::ReadRegister8(uint8_t address) {
        uint8_t ret = 0;

        ReadRegisterN(address, &ret, 1);

        return ret;
    }

    uint16_t TempProbe::ReadRegister16(uint8_t address) {
        uint8_t buf[2] = { 0, 0 };

        ReadRegisterN(address, buf, 2);

        uint16_t ret = buf[0];
        ret <<= 8;
        ret |= buf[1];

        return ret;
    }

    void TempProbe::WriteRegister8(uint8_t address, uint8_t data) {
        address |= 0x80;

    #ifdef RPI_PI

        digitalWrite(this->m_Ce0Pin, LOW);

        wiringPiSPIDataRW(this->k_Channel, static_cast<char*>(&address), 1);
        wiringPiSPIDataRW(this->k_Channel, static_cast<char*>(&data), 1);

        digitalWrite(this->m_Ce0Pin, HIGH);

    #endif
    }

    uint8_t TempProbe::ReadFault() {
        return ReadRegister8(FAULT_STATUS_REG);
    }

    void TempProbe::ClearFault() {
        uint8_t t = ReadRegister8(CONFIG_REG);

        t &= ~0x2C;
        t |= CONFIG_FAULT_STATUS;

        WriteRegister8(CONFIG_REG, t);
    }

    void TempProbe::CompareFault() {
        switch (ReadFault()) {
            case FAULT_NONE:
                std::cerr << "No errors detected." << std::endl; break;

            case FAULT_HIGH_THRESHOLD:
                std::cerr << "Measured resistance greater than High Fault Threshold value." << std::endl; break;

            case FAULT_LOW_THRESHOLD:
                std::cerr << "Measured resistance less than Low Fault Threshold value." << std::endl; break;

            case FAULT_REF_IN_LOW:
                std::cerr << "vREFIN > 0.85 x vBIAS." << std::endl; break;

            case FAULT_REF_IN_HIGH:
                std::cerr << "vRERFIN < 0.85 X vBIAS (FORCE - open)." << std::endl; break;

            case FAULT_RTD_IN_LOW:
                std::cerr << "vRTRIN- < 0.85 X vBIAS (FORCE - open)." << std::endl; break;

            case FAULT_OV_UV:
                std::cerr << "Any protected input voltage > vDD or < GND1." << std::endl; break;
        }
    }

    void TempProbe::EnableBias(bool enable) {
        uint8_t t = ReadRegister8(CONFIG_REG);

        if (enable)
            t |= CONFIG_BIAS;
        else
            t &= ~CONFIG_BIAS;

        WriteRegister8(CONFIG_REG, t);
    }

    void TempProbe::AutoConvert(bool enable) {
        uint8_t t = ReadRegister8(CONFIG_REG);

        if (enable)
            t |= CONFIG_MODE_AUTO;
        else
            t &= ~CONFIG_MODE_AUTO;

        WriteRegister8(CONFIG_REG, t);
    }

    void TempProbe::SetWires(uint8_t numWires) {
        uint8_t t = ReadRegister8(CONFIG_REG);

        if (numWires == 3)
            t |= CONFIG_3_WIRE;
        else
            t &= ~CONFIG_3_WIRE;

        WriteRegister8(CONFIG_REG, t);
    }

    void TempProbe::SetFilter(uint8_t filterHz) {
        uint8_t t = ReadRegister8(CONFIG_REG);

        if (filterHz == 50)
            t |= CONFIG_FILTER_50HZ;
        else
            t &= ~CONFIG_FILTER_50HZ;

        WriteRegister8(CONFIG_REG, t);
    }

    uint16_t TempProbe::ReadRTD() {
        ClearFault();
        EnableBias(true);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        uint8_t t = ReadRegister8(CONFIG_REG);

        t |= CONFIG_1_SHOT;

        WriteRegister8(CONFIG_REG, t);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        uint16_t rtd = ReadRegister16(RTD_MSB_REG);

        rtd >>= 1;

        EnableBias(false);

        return rtd;
    }

    void TempProbe::Init() {
        // ==================== WiringPi Pin Setup ====================
        // 
    #ifdef RPI_PI

        if ((this->m_fd = wiringPiSPISetup(this->k_Channel, this->k_hz)) == -1)
            throw std::runtime_error("Failed to setup Temp Probe SPI bus!");

        pinMode(this->m_Ce0Pin, OUTPUT);
        digitalWrite(this->m_Ce0Pin, HIGH);

    #endif

        SetWires(WIRE_COUNT);
        EnableBias(false);
        AutoConvert(false);

        ClearFault();

        SetFilter(FILTER_HZ);
    }

    void TempProbe::Shutdown() {
        // ==================== WiringPi Pin Cleanup ====================
        //
    #ifdef RPI_PI

        wiringPiSPIClose(this->k_Channel);

    #endif
    }

    #define RTD_A     3.9083e-3f;
    #define RTD_B     -5.775e-7f;

    void TempProbe::Update() {
        // Calculate the temperature, 430 is the resistor.
        float rt = (ReadRTD() / 32768.0f) * 430.0f;
        constexpr float z1 = -RTD_A;
        constexpr float z2 = RTD_A * RTD_A - (4.0f * RTD_B);
        // 100 is the reference resistance.
        constexpr float z3 = (4.0f * RTD_B) / 100.0f;
        constexpr float z4 = 2.0f * RTD_B;

        float temp = z2 + (z3 * rt);
        temp = (sqrt(temp) + z1) / z4;
 
        if (temp >= 0) { // Save temp if it above 0 *C
            CompareFault();

            printf("Temperature C*: %.2f\n", temp);
        }
    }

}   // BB
