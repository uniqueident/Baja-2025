#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class EngineRPM : public Module {
    public:
        EngineRPM(SharedData* data, Physical signal);
        ~EngineRPM();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        static void EnginePulse();

    private:
        const Physical m_DataPin;

        static inline unsigned int s_Pulse = 0;

    };

} // BB
