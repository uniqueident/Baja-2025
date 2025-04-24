#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class GearShifter : public Module {
    public:
        GearShifter(SharedData* data, Physical reverse, Physical drive, Physical neutral);
        ~GearShifter();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:

        const Physical m_reverse;
        const Physical m_drive;
        const Physical m_neutral;

    };

} // BB
