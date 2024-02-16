#include "motor.hpp"

namespace Hardware
{

    Motor::Motor(fp32 kp, fp32 ki, fp32 kd)
    {
        pid_ctrl.kp = kp;
        pid_ctrl.ki = ki;
        pid_ctrl.kd = kd;
    }
    Motor::~Motor()
    {
    }

}  // namespace Hardware
