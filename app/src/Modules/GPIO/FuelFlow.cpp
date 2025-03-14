#include "FuelFlow.hpp"

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
        
    }

    void FuelFlow::Shutdown() {
        
    }

    void FuelFlow::Update() {
        
    }

}   // BB
