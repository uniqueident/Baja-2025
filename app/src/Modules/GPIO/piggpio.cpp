#include "math.h"
#include "rpigpio.h"


#define MAX31865_CONFIG_REG             0x00
#define MAX31865_CONFIG_BIAS            0x80
#define MAX31865_CONFIG_MODEAUTO        0x40
#define MAX31865_CONFIG_MODEOFF         0x00
#define MAX31865_CONFIG_1SHOT           0x20
#define MAX31865_CONFIG_3WIRE           0x10
#define MAX31865_CONFIG_24WIRE          0x00
#define MAX31865_CONFIG_FAULTSTAT       0x02
#define MAX31865_CONFIG_FILT50HZ        0x01
#define MAX31865_CONFIG_FILT60HZ        0x00

#define MAX31865_RTDMSB_REG             0x01
#define MAX31865_RTDLSB_REG             0x02
#define MAX31865_HFAULTMSB_REG          0x03
#define MAX31865_HFAULTLSB_REG          0x04
#define MAX31865_LFAULTMSB_REG          0x05
#define MAX31865_LFAULTLSB_REG          0x06
#define MAX31865_FAULTSTAT_REG          0x07

#define MAX31865_FAULT_HIGHTHRESH       0x80
#define MAX31865_FAULT_LOWTHRESH        0x40
#define MAX31865_FAULT_REFINLOW         0x20
#define MAX31865_FAULT_REFINHIGH        0x10
#define MAX31865_FAULT_RTDINLOW         0x08
#define MAX31865_FAULT_OVUV             0x04
#define MAX31865_FAULT_NONE             0x00

#define RTD_A                           3.9083e-3
#define RTD_B                           -5.775e-7
#define _MAX31865_RREF                  402.0f
#define _MAX31865_RNOMINAL              100.0f

#define MAX31865_WIRES                  3
#define MAX31865_FILTERHZ               50


MAX31865::MAX31865(uint8_t spiHandle, uint8_t cs_pin)
{
    this->_spiHandle = spiHandle;
    this->_cs_pin = cs_pin;

    gpioSetMode(this->_cs_pin, PI_OUTPUT);
    gpioWrite(this->_cs_pin, PI_HIGH);

    MAX31865_setWires(MAX31865_WIRES);
    MAX31865_enableBias(false);
    MAX31865_autoConvert(false);
    MAX31865_clearFault();
    MAX31865_setFilter(MAX31865_FILTERHZ);
}

void MAX31865::MAX31865_readRegisterN(uint8_t address, uint8_t *buffer, uint8_t n) {

    uint8_t tmp = 0xFF;
    address &= 0x7F;

    gpioWrite(this->_cs_pin, PI_LOW);

    spiWrite(this->_spiHandle, (char *) &address, 1);
    while (n--) {
        spiXfer(this->_spiHandle, (char*) &tmp, (char *) buffer, 1);
        buffer++;
    }

    gpioWrite(this->_cs_pin, PI_HIGH);
}

uint8_t MAX31865::MAX31865_readRegister8(uint8_t address) {

        uint8_t ret = 0;
        MAX31865_readRegisterN(address, &ret, 1);
        return ret;
}

uint16_t MAX31865::MAX31865_readRegister16(uint8_t address) {
    uint8_t buffer[2] = {0, 0};

    MAX31865_readRegisterN(address, buffer, 2);
    uint16_t ret = buffer[0];
    ret <<= 8;
    ret |= buffer[1];

    return ret;
}

void MAX31865::MAX31865_writeRegister8(uint8_t address, uint8_t data) {

    gpioWrite(this->_cs_pin, PI_LOW);
    address |= 0x80;

    spiWrite(this->_spiHandle, (char*) &address, 1);
    spiWrite(this->_spiHandle, (char*) &data, 1);

    gpioWrite(this->_cs_pin, PI_HIGH);
}

void MAX31865::MAX31865_readFault(){
    this->_fault = MAX31865_readRegister8(MAX31865_FAULTSTAT_REG);
}

void MAX31865::MAX31865_clearFault() {

        uint8_t t = MAX31865_readRegister8(MAX31865_CONFIG_REG);
        t &= ~0x2C;
        t |= MAX31865_CONFIG_FAULTSTAT;
        MAX31865_writeRegister8(MAX31865_CONFIG_REG, t);
}

void MAX31865::MAX31865_enableBias(bool enable) {

    uint8_t t = MAX31865_readRegister8(MAX31865_CONFIG_REG);
    if (enable) {
        t |= MAX31865_CONFIG_BIAS;
    }
    else {
        t &= ~MAX31865_CONFIG_BIAS;
    }
    MAX31865_writeRegister8(MAX31865_CONFIG_REG, t);
}

void MAX31865::MAX31865_autoConvert(bool enable) {

        uint8_t t = MAX31865_readRegister8(MAX31865_CONFIG_REG);
        if (enable) {
            t |= MAX31865_CONFIG_MODEAUTO;
        }
        else {
            t &= ~MAX31865_CONFIG_MODEAUTO;
        }
        MAX31865_writeRegister8(MAX31865_CONFIG_REG, t);
}

void MAX31865::MAX31865_setWires(uint8_t numWires) {

    uint8_t t = MAX31865_readRegister8(MAX31865_CONFIG_REG);
    if (numWires == 3) {
        t |= MAX31865_CONFIG_3WIRE;
    }
    else {
        t &= ~MAX31865_CONFIG_3WIRE;
    }
    MAX31865_writeRegister8(MAX31865_CONFIG_REG, t);
}

void MAX31865::MAX31865_setFilter(uint8_t filterHz) {
    uint8_t t = MAX31865_readRegister8(MAX31865_CONFIG_REG);
    if (filterHz == 50) {
        t |= MAX31865_CONFIG_FILT50HZ;
    }
    else {
        t &= ~MAX31865_CONFIG_FILT50HZ;
    }
    MAX31865_writeRegister8(MAX31865_CONFIG_REG, t);
}

uint16_t MAX31865::MAX31865_readRTD() {

    MAX31865_clearFault();
    MAX31865_enableBias(true);

    std::this_thread::sleep_for (std::chrono::milliseconds(10));

    uint8_t t = MAX31865_readRegister8(MAX31865_CONFIG_REG);

    t |= MAX31865_CONFIG_1SHOT;
    MAX31865_writeRegister8(MAX31865_CONFIG_REG, t);

    std::this_thread::sleep_for (std::chrono::milliseconds(10));

    uint16_t rtd = MAX31865_readRegister16(MAX31865_RTDMSB_REG);
    MAX31865_enableBias(false);
    rtd >>= 1;

    return rtd;
}

void MAX31865::MAX31865_readTemp() {

    float Z1, Z2, Z3, Z4, Rt, temp;

    Rt = MAX31865_readRTD();
    Rt /= 32768;
    Rt *= _MAX31865_RREF;
    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / _MAX31865_RNOMINAL;
    Z4 = 2 * RTD_B;
    temp = Z2 + (Z3 * Rt);
    temp = (sqrtf(temp) + Z1) / Z4;

    if (temp >= 0) {
        MAX31865_readFault();
        MAX31865_compareFault();
        // Assume the temperature is correct
        this->_MAX31865_tempC = temp;
        this->_MAX31865_tempF = (this->_MAX31865_tempC * 9.0f / 5.0f) + 32.0f;
    }
    else {
        Rt /= _MAX31865_RNOMINAL;
        Rt *= 100;

        float rpoly = Rt;

        temp = -242.02;
        temp += 2.2228 * rpoly;
        rpoly *= Rt;  // square
        temp += 2.5859e-3 * rpoly;
        rpoly *= Rt;  // ^3
        temp -= 4.8260e-6 * rpoly;
        rpoly *= Rt;  // ^4
        temp -= 2.8183e-8 * rpoly;
        rpoly *= Rt;  // ^5
        temp += 1.5243e-10 * rpoly;

        MAX31865_readFault();
        MAX31865_compareFault();
        this->_MAX31865_tempC = temp;
        this->_MAX31865_tempF = (this->_MAX31865_tempC * 9.0f / 5.0f) + 32.0f;
    }
}

void MAX31865::MAX31865_compareFault() {

    this->_faultText = "Unknown error has occured. Refer to the MAX31865 datasheet.";

    if (this->_fault == MAX31865_FAULT_NONE)
        this->_faultText = "No errors detected";

    if (this->_fault == MAX31865_FAULT_HIGHTHRESH)
        this->_faultText = "Measured resistance greater than High Fault Threshold value.";

    if (this->_fault == MAX31865_FAULT_LOWTHRESH)
       this->_faultText = "Measured resistance less than Low Fault Threshold value.";

    if (this->_fault == MAX31865_FAULT_REFINLOW)
        this->_faultText = "vREFIN > 0.85 x vBIAS.";

    if (this->_fault == MAX31865_FAULT_REFINHIGH)
        this->_faultText = "vRERFIN < 0.85 X vBIAS (FORCE - open).";

    if (this->_fault == MAX31865_FAULT_RTDINLOW)
        this->_faultText = "vRTRIN- < 0.85 X vBIAS (FORCE - open).";

    if (this->_fault == MAX31865_FAULT_OVUV)
        this->_faultText = "Any protected input voltage > vDD or < GND1.";

}