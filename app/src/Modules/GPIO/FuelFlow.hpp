#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    /**
     * @brief The base class for a Module. Pure virtual class.
     */
    class FuelFlow :public Module {
    public:
        FuelFlow(SharedData* data, Physical gpio);
        ~FuelFlow();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        static void FlowPulse();

    private:
        const Physical m_DataPin;

        static inline unsigned int s_Pulse = 0;

    };

} // BB