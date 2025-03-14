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
        int GetTemp();

        void SetSpeed(int speed);

    private:
        const Physical m_pwmPin;
        const Physical m_rpmPin;

        int m_CurrentSpeed;
        int m_StartTemp;

    };

} // BB