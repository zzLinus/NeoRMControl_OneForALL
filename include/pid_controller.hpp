#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__

#include "types.hpp"

namespace Pid
{

    class Pid_controller
    {
       public:
        Pid_controller() = delete;
        Pid_controller(uint8_t mode, const fp32 kp, const fp32 ki,const fp32 kd,const fp32 max_out, const fp32 max_iout);
        ~Pid_controller();

       private:
       public:
        uint8_t mode;
        // PID 三参数
        fp32 kp;
        fp32 ki;
        fp32 kd;

        fp32 max_out;   // 最大输出
        fp32 max_iout;  // 最大积分输出

        fp32 set;
        fp32 fdb;

        fp32 out;
        fp32 Pout;
        fp32 Iout;
        fp32 Dout;
        fp32 Dbuf[3];   // 微分项 0最新 1上一次 2上上次
        fp32 error[3];  // 误差项 0最新 1上一次 2上上次
       private:
    };

}  // namespace Pid

#endif
