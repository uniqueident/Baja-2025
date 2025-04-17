#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class TempProbe : public Module {
    public:
        TempProbe(SharedData* data, Physical clock, Physical miso, Physical mosi, Physical ce0);
        ~TempProbe();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

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
