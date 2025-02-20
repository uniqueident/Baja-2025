#include "TempProbe.hpp"

#include "Modules/Sensors/Pins.hpp"

// libs
#ifdef RPI_PI

    #include <wiringPi.h>
    #include <wiringPiSPI.h>

#endif

namespace BB {

    TempProbe::TempProbe(SharedData* data) : 
        Module(data)
    {
        
    }

    TempProbe::~TempProbe() {
        // ==================== WiringPi Pin Cleanup ====================
        //

    }

    void TempProbe::Init() {
        // ==================== WiringPi Pin Setup ====================
        //
    #ifdef RPI_PI

        wiringPiSPIDataRW(this->k_Channel, this->m_Buffer, 3);

    #endif
    }

    void TempProbe::Shutdown() {
        
    }

    void TempProbe::Update() {
        // ==================== WiringPi Actions ====================
        //
    #ifdef RPI_PI

        if (digitalRead(Physical::PIN_3) == HIGH)
            std::cout << "HIGH Signal" << std::endl;

    #endif
    }

}   // BB
