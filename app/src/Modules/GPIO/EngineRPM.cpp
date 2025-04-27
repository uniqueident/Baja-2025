#include "EngineRPM.hpp"

// std
#include <chrono>

#ifdef RPI_PI

    #include <wiringPi.h>

#endif

namespace BB {

    EngineRPM::EngineRPM(SharedData* data, Physical signal) :
        Module(data),
        m_DataPin(signal)
    {
        this->p_Data->RegisterPin(this->m_DataPin);
    }

    EngineRPM::~EngineRPM()
    {
        this->p_Data->UnregisterPin(this->m_DataPin);
    }

    void EngineRPM::Init() {
    #ifdef RPI_PI

        pinMode(this->m_DataPin, INPUT);

        wiringPiISR(this->m_DataPin, INT_EDGE_RISING, &EngineRPM::EnginePulse);

    #endif

        this->m_PrevTime = std::chrono::steady_clock::now();
    }

    void EngineRPM::Shutdown() {
    #ifdef RPI_PI

        wiringPiISRStop(this->m_DataPin);

        pinMode(this->m_DataPin, PM_OFF);

    #endif
    }

    #define TIRE_DIAMETER 20.0f // In inches
    #define GEAR_RATIO    31.0f

    void EngineRPM::Update() {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - this->m_PrevTime;

        if (elapsed.count() >= 1.0f) {
            this->m_PrevTime = now;

            this->p_Data->engineRPM = static_cast<float>(s_Pulse) * 60.0f;

            s_Pulse = 0;

            // This supposedly is a correct conversion.
            // 
            this->p_Data->milesPerHour = (this->p_Data->engineRPM * TIRE_DIAMETER) / (GEAR_RATIO * 336);
        }
    }

    void EngineRPM::EnginePulse() {
        s_Pulse++;
    }

} // namespace BB
