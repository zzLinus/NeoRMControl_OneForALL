#include "motor.hpp"

namespace Hardware
{

    Motor::Motor(fp32 kp, fp32 ki, fp32 kd)
    {
        pid.kp = kp;
        pid.ki = ki;
        pid.kd = kd;
    }

    Motor::~Motor()
    {
    }

}  // namespace Hardware
