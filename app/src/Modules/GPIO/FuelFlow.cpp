#include "FuelFlow.hpp"

#ifdef RPI_PI

    #include <wiringPi.h>

#endif

namespace BB {

    FuelFlow::FuelFlow(SharedData* data, Physical gpio) :
        Module(data),
        m_DataPin(gpio)
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

    #endif
    }

    void FuelFlow::Shutdown() {
    #ifdef RPI_PI

        wiringPiISRStop(this->m_DataPin);

        pinMode(this->m_DataPin, PM_OFF);

    #endif
    }

    void FuelFlow::Update() {
        if (s_Pulse >= 10) {
            // The fuel flow sensor sends a pulse every 2.5 mL so every 10 pulses is 25 mL.
            //
            this->p_Data->fuel -= 25;

            s_Pulse = 0;
        }
    }

    void FuelFlow::FlowPulse() {
        s_Pulse++;
    }

}   // BB
