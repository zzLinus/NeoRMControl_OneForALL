#ifndef __CHASSIS__
#define __CHASSIS__
#include "chassis_ctrl.hpp"

namespace Chassis
{
    class Chassis_task
    {
       public:
        Chassis_task();
        ~Chassis_task();
        void task(void);
        void init(void);
		void update_feedback(void);

       private:

       public:
       private:
        Chassis_ctrl *cc;               // NOTE: cc stand for chassis control
    };

}  // namespace Chassis

#endif
