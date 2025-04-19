#include "EngineRPM.hpp"

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
    }

    void EngineRPM::Shutdown() {
    #ifdef RPI_PI

        wiringPiISRStop(this->m_DataPin);

        pinMode(this->m_DataPin, PM_OFF);

    #endif
    }

    void EngineRPM::Update() {
        // TODO
    }

    void EngineRPM::EnginePulse() {
        s_Pulse++;
    }

} // namespace BB
