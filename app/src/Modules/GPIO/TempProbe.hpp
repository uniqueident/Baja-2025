#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

#include <stdint.h>

namespace BB {

    class TempProbe : public Module {
    public:
        TempProbe(SharedData* data, Physical clock, Physical miso, Physical mosi, Physical ce0);
        ~TempProbe();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        void ReadRegisterN(uint8_t address, uint8_t* buffer, uint8_t n);
        uint8_t ReadRegister8(uint8_t address);
        uint16_t ReadRegister16(uint8_t address);

        void WriteRegister8(uint8_t address, uint8_t data);

        uint8_t ReadFault();
        void ClearFault();

        void CompareFault();

        void EnableBias(bool enable);
        void AutoConvert(bool enable);
        void SetWires(uint8_t numWires);
        void SetFilter(uint8_t filterHz);

        uint16_t ReadRTD();

    private:
        const int k_Channel = 0;
        // The clock speed must be within 500,000 and 32,000,000 Hz.
        //
        const int k_hz = 500'000;

        const Physical m_ClockPin;
        const Physical m_MisoPin;
        const Physical m_MosiPin;
        const Physical m_Ce0Pin;

        int m_fd;

    };  // TempProbe

}   // BB
