#ifndef __CHASSIS__
#define __CHASSIS__
#include "chassis_ctrl.hpp"
#include "kb_ctrl.hpp"

namespace Chassis
{
    class Chassis_task
    {
       public:
        Chassis_task();
        ~Chassis_task();
        void task(Types::RC_ctrl_t* rc_ctrller, Types::debug_info_t* debug);

       private:
       public:
        Chassis_ctrl* cc;  // NOTE: cc stand for chassis control
       private:
    };

}  // namespace Chassis

#endif
