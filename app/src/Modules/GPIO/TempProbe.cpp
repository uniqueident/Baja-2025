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

    /**
     * - Register Addresses and POR State
     * -------------------------------------------------------------------
     * | Register Name  | Read Address | Write Address | POR State | R/W |
     * -------------------------------------------------------------------
     * | Configuration  | 0x00         | 0x80          | 0x00      | R/W |
     * | RTD MSBs       | 0x01         |               | 0x00      | R   |
     * | RTD LSBs       | 0x02         |               | 0x00      | R   |
     * | High Fault MSB | 0x03         | 0x83          | 0xFF      | R/W |
     * | High Fault LSB | 0x04         | 0x84          | 0xFF      | R/W |
     * | Low Fault MSB  | 0x05         | 0x85          | 0x00      | R/W |
     * | Low Fault LSB  | 0x06         | 0x86          | 0x00      | R/W |
     * | Fault Status   | 0x07         |               | 0x00      | R   |
     * -------------------------------------------------------------------
     */

    #define CONFIG_REG_READ      0x00
    #define CONFIG_REG_WRITE     0x80
    #define CONFIG_REG_PW        0x00

    #define RTD_MSB_READ         0x01
    #define RTD_MSB_PW           0x00
    #define RTD_LSB_READ         0x02
    #define RTD_LSB_PW           0x00

    #define HIGH_FAULT_MSB_READ  0x03
    #define HIGH_FAULT_MSB_WRITE 0x83
    #define HIGH_FAULT_MSB_PW    0xFF

    #define HIGH_FAULT_LSB_READ  0x04
    #define HIGH_FAULT_LSB_WRITE 0x84
    #define HIGH_FAULT_LSB_PW    0xFF

    #define LOW_FAULT_MSB_READ   0x05
    #define LOW_FAULT_MSB_WRITE  0x85
    #define LOW_FAULT_MSB_PW     0x00

    #define LOW_FAULT_LSB_READ   0x06
    #define LOW_FAULT_LSB_WRITE  0x86
    #define LOW_FAULT_LSB_PW     0x00

    #define FAULT_STATUS         0x07
    #define FAULT_STATUS_PW      0x00

    /**
     * - Configuration Register Definition
     * ------------------------------------------------------------------
     * | D0 | 50 / 60 Hz Filter Select      | 1 = 50 Hz  | 0 = 60 Hz    |
     * | D1 | Fault Status Clear            | 1 = Clear  |              |
     * | D2 | Fault Detection Cycle Control |            |              |
     * | D3 | Fault Detection Cycle Control |            |              |
     * | D4 | 3-Wire vs 2/4-Wire            | 1 = 3-Wire | 0 = 2/4-Wire |
     * | D5 | 1-Shot Auto Clear             | 1 = 1-Shot |              |
     * | D6 | Conversion Mode               | 1 = Auto   |              |
     * | D7 | V-Bias                        | 1 = On     | 0 = Off      |
     * ------------------------------------------------------------------
     */

    #define FILTER_SELECT_50HZ   0b00000001
    #define FILTER_SELECT_60HZ   0b00000000

    #define FAULT_STATUS_CLEAR   0b00000010

    #define WIRE_SELECT_3        0b00010000
    #define WIRE_SELECT_2_4      0b00000000

    #define ONE_SHOT_CLEAR       0b00100000

    #define CONVERSION_MODE_AUTO 0b01000000

    #define V_BIAS_ENABLE        0b10000000
    #define V_BIAS_DISABLE       0b00000000

    /**
     * - Fault-Detection Cycle Control Bits
     * --------------------------------------------------------------------------------------------------------------------------------------------------
     * | D3 | D2 | Configuration Register Write | Write Action                             | Read Meaning                                               |
     * --------------------------------------------------------------------------------------------------------------------------------------------------
     * | 0  | 0  | 0bXXXX00XX                   | No action                                | Fault detection finished                                   |
     * | 0  | 1  | 0b100X010X                   | Fault detection with automatic delay     | Automatic fault detection still running                    |
     * | 1  | 0  | 0b100X100X                   | Run fault detection with manual delay    | Manual cycle 1 still running; waiting for user to write 11 |
     * | 1  | 1  | 0b100X110X                   | Finish fault detection with manual delay | Manual cycle 2 still running                               |
     * --------------------------------------------------------------------------------------------------------------------------------------------------
     */

    #define NO_FAULT_ACTION        0b00000000

    #define AUTO_FAULT_DETECTION   0b10000100

    #define MANUAL_FAULT_DETECTION 0b10001000

    #define FINISH_FAULT_DETECTION 0b10001100

    /**
     * - Fault Status Register Definition
     * ----------------------------------------------
     * | D2 | Overvoltage / undervoltage fault      |
     * | D3 | RTD-In < 0.85 x V-Bias (Force - Open) |
     * | D4 | REF-In < 0.85 x V-Bias (Force - Open) |
     * | D5 | REF-In > 0.85 x V-Bias                |
     * | D6 | RTD Low Threshold                     |
     * | D7 | RTD High Threshold                    |
     * ----------------------------------------------
     */

    #define FAULT_NONE           0b00000011
    #define FAULT_OV_UV          0b00000100
    #define FAULT_RTD_IN_LOW     0b00001000
    #define FAULT_REF_IN_LOW     0b00010000
    #define FAULT_REF_IN_HIGH    0b00100000
    #define FAULT_HIGH_THRESHOLD 0b01000000
    #define FAULT_LOW_THRESHOLD  0b10000000


    void TempProbe::ReadRegisterN(uint8_t address, uint8_t* buffer, uint8_t n) {
        address &= 0x7F;

    #ifdef RPI_PI

        digitalWrite(this->m_Ce0Pin, LOW);

        wiringPiSPIDataRW(this->k_Channel, &address, n);

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

        wiringPiSPIDataRW(this->k_Channel, &address, 1);
        wiringPiSPIDataRW(this->k_Channel, &data, 1);

        digitalWrite(this->m_Ce0Pin, HIGH);

    #endif
    }

    uint8_t TempProbe::ReadFault() {
        return ReadRegister8(FAULT_STATUS);
    }

    void TempProbe::ClearFault() {
        uint8_t t = ReadRegister8(CONFIG_REG_READ);

        t &= ~0x2C;
        t |= FAULT_STATUS_CLEAR;

        WriteRegister8(CONFIG_REG_WRITE, t);
    }

    void TempProbe::CompareFault() {
        switch (ReadFault() & (~FAULT_NONE)) {
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
        uint8_t t = ReadRegister8(CONFIG_REG_READ);

        if (enable)
            t |= V_BIAS_ENABLE;
        else
            t &= ~V_BIAS_ENABLE;

        WriteRegister8(CONFIG_REG_WRITE, t);
    }

    void TempProbe::AutoConvert(bool enable) {
        uint8_t t = ReadRegister8(CONFIG_REG_READ);

        if (enable)
            t |= CONVERSION_MODE_AUTO;
        else
            t &= ~CONVERSION_MODE_AUTO;

        WriteRegister8(CONFIG_REG_WRITE, t);
    }

    void TempProbe::SetWires(uint8_t numWires) {
        uint8_t t = ReadRegister8(CONFIG_REG_READ);

        if (numWires == 3)
            t |= WIRE_SELECT_3;
        else
            t &= ~WIRE_SELECT_3;

        WriteRegister8(CONFIG_REG_WRITE, t);
    }

    void TempProbe::SetFilter(uint8_t filterHz) {
        uint8_t t = ReadRegister8(CONFIG_REG_READ);

        if (filterHz == 50)
            t |= FILTER_SELECT_50HZ;
        else
            t &= ~FILTER_SELECT_50HZ;

        WriteRegister8(CONFIG_REG_WRITE, t);
    }

    uint16_t TempProbe::ReadRTD() {
        ClearFault();
        EnableBias(true);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        uint8_t t = ReadRegister8(CONFIG_REG_READ);

        t |= ONE_SHOT_CLEAR;

        WriteRegister8(CONFIG_REG_WRITE, t);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        uint16_t rtd = ReadRegister16(RTD_MSB_READ);

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

        SetWires(3);
        EnableBias(false);
        AutoConvert(false);

        ClearFault();

        SetFilter(50);
    }

    void TempProbe::Shutdown() {
        // ==================== WiringPi Pin Cleanup ====================
        //
    #ifdef RPI_PI

        wiringPiSPIClose(this->k_Channel);

    #endif
    }

    #define RTD_A     3.9083e-3f
    #define RTD_B     -5.775e-7f

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
