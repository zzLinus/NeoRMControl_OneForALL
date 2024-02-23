#ifndef __KB_CTRL__
#define __KB_CTRL__

#include "imtui/imtui-impl-ncurses.h"
#include "imtui/imtui.h"
#include "ramp_controller.hpp"
#include "types.hpp"
#include <chrono>

namespace Input
{
    class Kb_ctrl
    {
       public:
        Kb_ctrl();
        ~Kb_ctrl();
        void task();
        void ramp_calc(Types::ramp_t *ramp_source_type, fp32 input);

       private:
       public:
        Types::RC_ctrl_t *rc_ctrl;
        Types::debug_info_t *debug;
        Ramp::Ramp_ctrller *kb_vx_ramp;  // 用于键盘控制的斜波函数
        Ramp::Ramp_ctrller *kb_vy_ramp;  // 用于键盘控制的斜波函数

       private:
    };
}  // namespace Input

#endif
