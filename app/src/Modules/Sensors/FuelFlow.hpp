#pragma once

#include "Modules/Module.hpp"

namespace BB {

    /**
     * @brief The base class for a Module. Pure virtual class.
     */
    class FuelFlow :public Module {
        public:
        FuelFlow(SharedData* data);
        ~FuelFlow();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;
    };

} // BB