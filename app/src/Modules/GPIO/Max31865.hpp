#pragma once

#include <cstdint>


#define MAX31865_CONFIG_REG         0x00
#define MAX31865_CONFIG_BIAS        0x80
#define MAX31865_CONFIG_MODEAUTO    0x40
#define MAX31865_CONFIG_1SHOT       0x20
#define MAX31865_CONFIG_3WIRE       0x10
#define MAX31865_CONFIG_FAULTSTAT   0x02
#define MAX31865_CONFIG_FILT50HZ    0x01

#define MAX31865_RTDMSB_REG         0x01
#define MAX31865_RTDLSB_REG         0x02
#define MAX31865_HFAULTMSB_REG      0x03
#define MAX31865_HFAULTLSB_REG      0x04
#define MAX31865_LFAULTMSB_REG      0x05
#define MAX31865_LFAULTLSB_REG      0x06
#define MAX31865_FAULTSTAT_REG      0x07

#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7

class Max31865 {
public:
    Max31865(int spiChannel = 0, int spiClock = 1000000);
    ~Max31865();

    bool Begin(int wires = 2);

    uint16_t ReadRTD();
    float TemperatureC(float nominal, float ref);

    void EnableBias(bool enable);
    void AutoConvert(bool enable);
    void SetWires(int wires);

    void ClearFault();
    uint8_t ReadFault();

    void SetThresholds(uint16_t lower, uint16_t upper);

private:
    int m_Channel;
    int m_Speed;

    void WriteRegister8(uint8_t addr, uint8_t data);

    uint8_t ReadRegister8(uint8_t addr);
    uint8_t ReadRegister16(uint8_t addr);

    void ReadRegisterN(uint8_t addr, uint8_t* buffer, uint8_t n);

};  // Max31865
