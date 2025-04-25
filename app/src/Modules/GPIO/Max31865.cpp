#include "Max31865.hpp"

#include <cmath>
#include <cstring>
#include <unistd.h>

#ifdef RPI_PI

    #include <wiringPiSPI.h>

#endif

#include <cstdint>

Max31865::Max31865(int spiChannel, int spiClock)
    : m_Channel(spiChannel),
      m_Speed(spiClock)
{
    
}

Max31865::~Max31865()
{
    
}

bool Max31865::Begin(int wires) {
    SetWires(wires);
    EnableBias(false);
    AutoConvert(false);
    SetThresholds(0, 0xFFFF);

    ClearFault();

    return true;
}

uint16_t Max31865::ReadRTD() {
    ClearFault();
    EnableBias(true);

    usleep(10000); // 10ms

    uint8_t config = ReadRegister8(MAX31865_CONFIG_REG);

    config |= MAX31865_CONFIG_1SHOT;

    WriteRegister8(MAX31865_CONFIG_REG, config);

    usleep(65000); // 65ms

    uint16_t rtd = ReadRegister16(MAX31865_RTDMSB_REG);

    EnableBias(false);

    return rtd >> 1; // Strip Fault Bit
}

float Max31865::TemperatureC(float nominal, float ref) {
    uint16_t rtd = ReadRTD();
    float Rt = rtd;
    Rt /= 32768;
    Rt *= ref;

    float Z1 = -RTD_A;
    float Z2 = RTD_A * RTD_A - (4 * RTD_B);
    float Z3 = (4 * RTD_B) / nominal;
    float Z4 = 2 * RTD_B;

    float temp = Z2 + (Z3 * Rt);
    temp = (std::sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
        return temp;

    // Polynomial approximation for < 0°C
    Rt /= nominal;
    Rt *= 100;

    float rpoly = Rt;
    temp = -242.02 + 2.2228 * rpoly;
    rpoly *= Rt;
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rt;
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rt;
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rt;
    temp += 1.5243e-10 * rpoly;

    return temp;
}

void Max31865::EnableBias(bool enable) {
    uint8_t config = ReadRegister8(MAX31865_CONFIG_REG);

    if (enable)
        config |= MAX31865_CONFIG_BIAS;
    else
        config &= ~MAX31865_CONFIG_BIAS;

    WriteRegister8(MAX31865_CONFIG_REG, config);
}

void Max31865::AutoConvert(bool enable) {
    uint8_t config = ReadRegister8(MAX31865_CONFIG_REG);

    if (enable)
        config |= MAX31865_CONFIG_MODEAUTO;
    else
        config &= ~MAX31865_CONFIG_MODEAUTO;

    WriteRegister8(MAX31865_CONFIG_REG, config);
}

void Max31865::SetWires(int wires) {
    uint8_t config = ReadRegister8(MAX31865_CONFIG_REG);

    if (wires == 3)
        config |= MAX31865_CONFIG_3WIRE;
    else
        config &= ~MAX31865_CONFIG_3WIRE;

    WriteRegister8(MAX31865_CONFIG_REG, config);
}

void Max31865::ClearFault() {
    uint8_t config = ReadRegister8(MAX31865_CONFIG_REG);

    config &= ~0x2C;
    config |= MAX31865_CONFIG_FAULTSTAT;

    WriteRegister8(MAX31865_CONFIG_REG, config);
}

uint8_t Max31865::ReadFault() {
    return ReadRegister8(MAX31865_FAULTSTAT_REG);
}

void Max31865::SetThresholds(uint16_t lower, uint16_t upper) {
    WriteRegister8(MAX31865_HFAULTMSB_REG, upper >> 8);
    WriteRegister8(MAX31865_LFAULTMSB_REG, lower >> 8);

    WriteRegister8(MAX31865_HFAULTLSB_REG, upper & 0xFF);
    WriteRegister8(MAX31865_LFAULTLSB_REG, lower & 0xFF);
}

void Max31865::WriteRegister8(uint8_t addr, uint8_t data) {
    uint8_t tx[2] = { static_cast<uint8_t>(addr | 0x80), data };

#ifdef RPI_PI

    wiringPiSPIDataRW(this->m_Channel, tx, 2);

#endif
}

uint8_t Max31865::ReadRegister8(uint8_t addr) {
    uint8_t rx[2] = { static_cast<uint8_t>(addr & 0x7F), 0x00 };

#ifdef RPI_PI

    wiringPiSPIDataRW(this->m_Channel, rx, 2);

#endif

    return rx[1];
}

uint8_t Max31865::ReadRegister16(uint8_t addr) {
    uint8_t buff[2] = { 0 };

    ReadRegisterN(addr, buff, 2);

    return (buff[0] << 8 | buff[1]);
}

void Max31865::ReadRegisterN(uint8_t addr, uint8_t* buffer, uint8_t n) {
    uint8_t* tx = new uint8_t[n + 1];

    tx[0] = addr & 0x7F;

    memset(tx + 1, 0, n);
#ifdef RPI_PI

    wiringPiSPIDataRW(this->m_Channel, tx, n + 1);

#endif

    memcpy(buffer, tx + 1, n);
}
