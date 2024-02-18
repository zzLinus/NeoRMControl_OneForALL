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
        Motor();
        ~Motor();

       private:
        Pid::Pid_controller *getPid();

       public:
        Pid::Pid_controller *pid_ctrler;
        // TODO: can bus api related
        motor_can_message *motor_measure;
        fp32 accel;
        fp32 speed;
        fp32 speed_set;
        int16_t give_current;

       private:
    };
}  // namespace Hardware

#endif
