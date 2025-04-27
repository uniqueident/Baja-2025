#include "FanControl.hpp"

// libs
#ifdef RPI_PI

    #include <wiringPi.h>

    #include <fcntl.h>
    #include <unistd.h>

#endif

namespace BB {

    FanControl::FanControl(SharedData* data, Physical pwm, Physical rpm) :
        Module(data),
        m_pwmPin(pwm),
        m_rpmPin(rpm)
    {
        this->p_Data->RegisterPin(this->m_pwmPin);
        this->p_Data->RegisterPin(this->m_rpmPin);
    }

    FanControl::~FanControl()
    {
        this->p_Data->UnregisterPin(this->m_pwmPin);
        this->p_Data->UnregisterPin(this->m_rpmPin);
    }

    // Target Clock Speed for Noctua NF-A4x20 is 25kH according to https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf

    #define PWM_RANGE 1024U
    #define PWM_DIVISOR 192U

    #define MIN_PI_TEMP 35.0f
    #define MAX_PI_TEMP 70.0f
    #define PI_TEMP_RANGE (MAX_PI_TEMP - MIN_PI_TEMP)

    void FanControl::Init() {
    #ifdef RPI_PI

        pwmWrite(this->m_pwmPin, 0);

        pinMode(this->m_pwmPin, PWM_OUTPUT);

        pwmSetMode(PWM_MODE_MS);

        // Default divider is 32
        // 
        pwmSetClock(PWM_DIVISOR);

        // Default range is 1024
        // 
        pwmSetRange(PWM_RANGE);

    #endif
    }

    void FanControl::Shutdown() {
    #ifdef RPI_PI

        pwmWrite(this->m_pwmPin, 0);

        pinMode(this->m_pwmPin, PM_OFF);

    #endif
    }

    // s_Counter is a simple counter to help spread out the updates.
    //
    static unsigned int s_Counter = 0;

    void FanControl::Update() {
        if (s_Counter++ >= 1'000) {
            this->p_Data->pi_Heat = GetTemp();

            if (this->p_Data->pi_Heat >= MAX_PI_TEMP)
                SetSpeed(1.0f);
            else if (this->p_Data->pi_Heat >= MIN_PI_TEMP)
                SetSpeed((this->p_Data->pi_Heat - MIN_PI_TEMP) / PI_TEMP_RANGE);
            else
                SetSpeed(0.0f);

            s_Counter = 0;
        }
    }

    float FanControl::GetTemp() {
        float temp = 0.0f;

    #ifdef RPI_PI

        char buf[10];

        int fd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
        read(fd, buf, sizeof(buf));
        close(fd);

        sscanf(buf, "%f", &temp);

    #endif

        return temp / 1000;
    }

    void FanControl::SetSpeed(float speed) {
        speed = speed * PWM_RANGE;

    #ifdef RPI_PI

        pwmWrite(this->m_pwmPin, static_cast<int>(speed));

    #endif
    }

    int FanControl::GetSpeed() {
        return 0;
    }

} // namespace BB
