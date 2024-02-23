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
        void task(Input::Kb_ctrl *kb_ctrl);

       private:
       public:
       private:
        Chassis_ctrl *cc;  // NOTE: cc stand for chassis control
    };

}  // namespace Chassis

#endif
