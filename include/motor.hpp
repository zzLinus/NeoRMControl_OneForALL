#ifndef __MOTOR__
#define __MOTOR__

#include "types.hpp"

namespace Hardware
{

    class Motor
    {
       public:
        Motor() = delete;
        Motor(fp32 kp, fp32 ki, fp32 kd);
        ~Motor();

       private:
        PID *getPid();

       public:
        PID pid;

        typedef struct
        {
            uint16_t ecd;
            int16_t speed_rpm;
            int16_t given_current;
            uint8_t temperate;
            int16_t last_ecd;
        } motor_can_message;

       private:
    };
}  // namespace Hardware

#endif
