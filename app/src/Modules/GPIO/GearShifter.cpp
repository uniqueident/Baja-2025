#include "GearShifter.hpp"

#include "Core/SharedData.hpp"

#ifdef RPI_PI

    #include <wiringPi.h>

#endif

namespace BB {

    GearShifter::GearShifter(SharedData* data, Physical park, Physical neutral, Physical gear1, Physical gear2) :
        Module(data),
        m_Park(park),
        m_Neutral(neutral),
        m_1(gear1),
        m_2(gear2)
    {
        this->p_Data->RegisterPin(this->m_Park);
        this->p_Data->RegisterPin(this->m_Neutral);
        this->p_Data->RegisterPin(this->m_1);
        this->p_Data->RegisterPin(this->m_2);
    }

    GearShifter::~GearShifter()
    {
        this->p_Data->UnregisterPin(this->m_Park);
        this->p_Data->UnregisterPin(this->m_Neutral);
        this->p_Data->UnregisterPin(this->m_1);
        this->p_Data->UnregisterPin(this->m_2);
    }

    void GearShifter::Init() {
    #ifdef RPI_PI

        pinMode(this->m_Park, INPUT);
        pinMode(this->m_Neutral, INPUT);
        pinMode(this->m_1,INPUT);
        pinMode(this->m_2,INPUT);

    #endif
    }

    void GearShifter::Shutdown() {
    #ifdef RPI_PI

        pinMode(this->m_Park, PM_OFF);
        pinMode(this->m_Neutral, PM_OFF);
        pinMode(this->m_1, PM_OFF);
        pinMode(this->m_2, PM_OFF);

    #endif
    }

    void GearShifter::Update() {
    #ifdef RPI_PI

        if(digitalRead(this->m_Park))
            this->p_Data->gearPosition = GearPosition::PARK;
        else if(digitalRead(this->m_Neutral))
            this->p_Data->gearPosition = GearPosition::NEUTRAL;
        else if(digitalRead(this->m_1))
            this->p_Data->gearPosition = GearPosition::ONE;
        else if(digitalRead(this->m_2))
            this->p_Data->gearPosition = GearPosition::TWO;
        else
            this->p_Data->gearPosition = GearPosition::REVERSE;

    #endif
    }

} // namespace BB
