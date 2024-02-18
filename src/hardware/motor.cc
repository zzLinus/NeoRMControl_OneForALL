#include "motor.hpp"

namespace Hardware
{

    Motor::Motor()
    {
        // TODO: get chassis motor data point,  initialize motor speed PID
        // 获取底盘电机数据指针，初始化PID
        pid_ctrler = new Pid::Pid_controller(
            Config::PID_POSITION,
            Config::M3505_MOTOR_SPEED_PID_KP,
            Config::M3505_MOTOR_SPEED_PID_KI,
            Config::M3505_MOTOR_SPEED_PID_KD,
            Config::M3505_MOTOR_SPEED_PID_MAX_OUT,
            Config::M3505_MOTOR_SPEED_PID_MAX_IOUT);
    }

    Motor::~Motor()
    {
        delete pid_ctrler;
    }

}  // namespace Hardware