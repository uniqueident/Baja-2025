#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class TempProbe : public Module {
    public:
        TempProbe(SharedData* data, Physical clock, Physical miso, Physical mosi, Physical ce0, Physical Gpio);
        ~TempProbe();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

        bool PinCheck(Physical pin);

    private:
        struct B10 {
            union {
                int val;

                unsigned char buf[3];

            };

        };  // B10

    private:
        const int k_Channel = 0;
        // The clock speed must be within 500,000 and 32,000,000 Hz.
        //
        const int k_hz = 1'000'000;

        const Physical m_ClockPin;
        const Physical m_MisoPin;
        const Physical m_MosiPin;
        const Physical m_Ce0Pin;
        const Physical m_GpioPin;

        float voltage;

        
        // The buffer must have the following format:
        //
        // Byte 1: Always 1 as to set the update signal for the MCP3008.
        // Byte 2: 8 + Channel Number << 4.
        //           8 marks the 4th bit for the data handling mode.
        //           Channel number is 0-7 to fit within 3 bits.
        //           Everything shifted 4 bits left to open right 4 bits.
        // Byte 3: Empty as this is reserved for recieved data, as well as
        //         the right 2 bits in Byte 2.
        //
        unsigned char m_Buffer[3];

        int m_fd;

    };  // TempProbe

}   // BB
