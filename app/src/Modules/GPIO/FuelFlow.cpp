#include "FuelFlow.hpp"

#ifdef RPI_PI

    #include <wiringPi.h>

#endif

namespace BB {

    FuelFlow::FuelFlow(SharedData* data, Physical flow, Physical reset) :
        Module(data),
        m_DataPin(flow),
        m_ResetPin(reset)
    {
        this->p_Data->RegisterPin(this->m_DataPin);
    }

    FuelFlow::~FuelFlow()
    {
        this->p_Data->UnregisterPin(this->m_DataPin);
    }

    void FuelFlow::Init() {
    #ifdef RPI_PI

        pinMode(this->m_DataPin, INPUT);

        wiringPiISR(this->m_DataPin, INT_EDGE_RISING, &FuelFlow::FlowPulse);

        pinMode(this->m_ResetPin, INPUT);

        wiringPiISR(this->m_ResetPin, INT_EDGE_RISING, &FuelFlow::ResetSignal);

    #endif
    }

    void FuelFlow::Shutdown() {
    #ifdef RPI_PI

        wiringPiISRStop(this->m_DataPin);

        pinMode(this->m_DataPin, PM_OFF);

        wiringPiISRStop(this->m_ResetPin);

        pinMode(this->m_ResetPin, PM_OFF);

    #endif
    }

    void FuelFlow::Update() {
        if (s_Reset)
            this->p_Data->ResetFuel();

        if (s_Pulse) {
            // The fuel flow sensor sends a pulse every 2.5 mL.
            //
            this->p_Data->fuel -= 2.5f * this->s_Pulse;

            s_Pulse = 0;
        }
    }

    void FuelFlow::FlowPulse() {
        s_Pulse++;
    }

    void FuelFlow::ResetSignal() {
        s_Reset = true;
    }

}   // BB
