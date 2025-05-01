#include "FuelFlow.hpp"

#include <chrono>

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
        this->p_Data->RegisterPin(this->m_ResetPin);
    }

    FuelFlow::~FuelFlow()
    {
        this->p_Data->UnregisterPin(this->m_DataPin);
        this->p_Data->UnregisterPin(this->m_ResetPin);
    }

    void FuelFlow::Init() {
        this->m_PrevTime = std::chrono::steady_clock::now();

    // #ifdef RPI_PI

    //     pinMode(this->m_DataPin, INPUT);

    //     wiringPiISR(this->m_DataPin, INT_EDGE_RISING, &FuelFlow::FlowPulse);

    //     pinMode(this->m_ResetPin, INPUT);

    //     wiringPiISR(this->m_ResetPin, INT_EDGE_BOTH, &FuelFlow::ResetSignal);

    // #endif
    }

    void FuelFlow::Shutdown() {
    // #ifdef RPI_PI

    //     wiringPiISRStop(this->m_DataPin);

    //     pinMode(this->m_DataPin, PM_OFF);

    //     wiringPiISRStop(this->m_ResetPin);

    //     pinMode(this->m_ResetPin, PM_OFF);

    // #endif
    }

    void FuelFlow::Update() {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - this->m_PrevTime;

        if (elapsed.count() >= 1.0f) {
            this->m_PrevTime = now;

            this->p_Data->fuel -= 20.0f / 60.0f;
        }

        // if (s_Reset){
        //     this->p_Data->ResetFuel();
        // }


        // if (s_Pulse) {
        //     // The fuel flow sensor sends a pulse every 2.5 mL.
        //     //
        //     this->p_Data->fuel -= 2.5f * this->s_Pulse;

        //     s_Pulse = 0;
        // }
    }

    void FuelFlow::FlowPulse() {
        // s_Pulse++;
    }

    void FuelFlow::ResetSignal() {
        // s_Reset = true;
    }

}   // BB
