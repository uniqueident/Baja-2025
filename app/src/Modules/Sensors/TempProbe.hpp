#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class TempProbe : public Module {
    public:
        TempProbe(SharedData* data);
        ~TempProbe();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        const int k_Channel = 0;
        const int k_mhz = 1'000'000;

        unsigned char m_Buffer[3] = {
            0, 8, 0
        };

    };  // TempProbe

}   // BB
