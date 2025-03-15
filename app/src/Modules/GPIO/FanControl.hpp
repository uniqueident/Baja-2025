#pragma once

#include "Core/SharedData.hpp"

#include "Modules/Module.hpp"

namespace BB {

    /**
     * @brief The base class for a Module. Pure virtual class.
     */
    class FanControl :public Module {
    public:
        FanControl(SharedData* data, Physical pwm, Physical rpm);
        ~FanControl();

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        // Get's the Raspberry Pi CPU temp.
        float GetTemp();

        void SetSpeed(float speed);
        int GetSpeed();

    private:
        // The pin that outputs the PWM signal.
        const Physical m_pwmPin;
        // The pin that reads the cycles from the fan to determine the RPM.
        const Physical m_rpmPin;

    };

} // BB