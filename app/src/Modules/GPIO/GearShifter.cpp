#include "GearShifter.hpp"
#include "Core/SharedData.hpp"
#include <wiringPi.h>

namespace BB {

    GearShifter::GearShifter(SharedData* data, Physical reverse, Physical drive, Physical neutral) :
        Module(data),
        m_reverse(reverse),
        m_drive(drive),
        m_neutral(neutral)
    {
        this->p_Data->RegisterPin(m_reverse);
        this->p_Data->RegisterPin(m_drive);
        this->p_Data->RegisterPin(m_neutral);
    }

    GearShifter::~GearShifter()
    {
        this->p_Data->UnregisterPin(m_reverse);
        this->p_Data->UnregisterPin(m_drive);
        this->p_Data->UnregisterPin(m_neutral);
    }

    void GearShifter::Init() {
        pinMode(m_reverse, INPUT);
        pinMode(m_drive,INPUT);
        pinMode(m_neutral, INPUT);
    }

    void GearShifter::Shutdown() {
        pinMode(m_reverse, PM_OFF);
        pinMode(m_drive, PM_OFF);
        pinMode(m_neutral, PM_OFF);
    }

    void GearShifter::Update() {

        if(digitalRead(m_reverse)){
            this->p_Data->gearPosition = REVERSE;
        }else if(digitalRead(m_drive)){
            this->p_Data->gearPosition = DRIVE;
        }else if(digitalRead(m_neutral)){
            this->p_Data->gearPosition = NEUTRAL;
        }else{
            this->p_Data->gearPosition = PARK;
        } 
    }

} // namespace BB
