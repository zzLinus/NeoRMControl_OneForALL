#ifndef __MOTOR__
#define __MOTOR__

#include "pid_controller.hpp"
#include "types.hpp"

namespace Hardware
{

    typedef struct
    {
        uint16_t ecd;
        int16_t speed_rpm;
        int16_t given_current;
        uint8_t temperate;
        int16_t last_ecd;
    } motor_can_message;

    class Motor
    {
       public:
        Motor() = delete;
        Motor(fp32 kp, fp32 ki, fp32 kd);
        ~Motor();

       private:
        Pid::Pid_controller *getPid();

       public:
        Pid::Pid_controller pid_ctrl;

       private:
    };
}  // namespace Hardware

#endif
