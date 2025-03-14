#include "FanControl.hpp"

// libs
#ifdef RPI_PI

    #include <wiringPi.h>

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

    #define MIN_DIVIDER 1U
    #define MAX_DIVIDER 1456U

    #define MIN_SPEED 10U
    #define MAX_SPEED 80U

    #define MIN_PI_TEMP 25.0f
    #define MAX_PI_TEMP 65.0f

    void FanControl::Init() {
    #ifdef RPI_PI

        pinMode(this->m_pwmPin, PWM_BAL_OUTPUT);

        // Default range is 1024
        // 
        pwmSetRange(1024);

        // Default divider is 32
        // 
        pwmSetClock(MIN_DIVIDER);

        pwmWrite(this->m_pwmPin, 0);

    #endif
    }

    void FanControl::Shutdown() {
    #ifdef RPI_PI

        pwmWrite(this->m_pwmPin, 0);

        pinMode(this->m_pwmPin, PM_OFF);

    #endif
    }

    void FanControl::Update() {
        // Get the current Pi temp.
        //
        // TODO: https://forums.raspberrypi.com/viewtopic.php?t=310977#p1859743

    }

    int FanControl::GetTemp() {
        int temp = 0;

    #ifdef RPI_PI
        char buf[10];

        int fd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
        read(fd, buf, sizeof(buf));
        close(fd);

        sscanf(buf, "%d", &temp);

    #endif

        return temp / 1000;
    }

    void FanControl::SetSpeed(int speed) {
        
    }

} // namespace BB
