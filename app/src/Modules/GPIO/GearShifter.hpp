#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class GearShifter : public Module {
    public:
        GearShifter(SharedData* data, Physical park, Physical neutral, Physical gear1, Physical gear2);
        ~GearShifter();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:

        const Physical m_Park;
        const Physical m_Neutral;
        const Physical m_1;
        const Physical m_2;

    };

} // BB
