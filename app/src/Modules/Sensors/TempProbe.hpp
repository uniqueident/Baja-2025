#pragma once


#include "Modules/Module.hpp"

namespace BB {

    class TempProbe : public Module {
    public:
        TempProbe(SharedData* data,Physical clock, Physical miso, Physical mosi, Physical ce0);
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

        int i =0;

    };  // TempProbe

}   // BB
