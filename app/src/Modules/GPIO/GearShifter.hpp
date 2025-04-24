#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class GearShifter : public Module {
    public:
        GearShifter(SharedData* data);
        ~GearShifter();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:

    };

} // BB
