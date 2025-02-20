#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    class TempProbe : public Module {
    public:
        TempProbe(SharedData* data);
        ~TempProbe();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    };  // TempProbe

}   // BB
