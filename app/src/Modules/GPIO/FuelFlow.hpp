#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

#include <chrono>

namespace BB {

    /**
     * @brief The base class for a Module. Pure virtual class.
     */
    class FuelFlow : public Module {
    public:
        FuelFlow(SharedData* data, Physical flow, Physical reset);
        ~FuelFlow();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        static void FlowPulse();
        static void ResetSignal();

    private:
        const Physical m_DataPin;
        const Physical m_ResetPin;

        std::chrono::steady_clock::time_point m_PrevTime;

        static inline unsigned int s_Pulse = 0;
        static inline bool s_Reset = false;

    };

} // BB
