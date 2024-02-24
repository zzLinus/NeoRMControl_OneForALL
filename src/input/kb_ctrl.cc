#include "kb_ctrl.hpp"

namespace Input
{
    Types::RC_ctrl_t *Kb_ctrl::rc_ctrl;
    Ramp::Ramp_ctrller *Kb_ctrl::kb_vx_ramp;
    Ramp::Ramp_ctrller *Kb_ctrl::kb_vy_ramp;

    Kb_ctrl::Kb_ctrl()
    {
        // HACK:键盘控制斜波函数初始化
        kb_vx_ramp = new Ramp::Ramp_ctrller();
        kb_vy_ramp = new Ramp::Ramp_ctrller();

        rc_ctrl = new Types::RC_ctrl_t();
    }

    Kb_ctrl::~Kb_ctrl()
    {
        delete kb_vx_ramp;
        delete kb_vy_ramp;
        delete rc_ctrl;
    }

    fp32 Kb_ctrl::event_handler(Types::Kb_event event, fp32 spdslider)
    {
        switch (event)
        {
            case Types::Kb_event::UP:
                rc_ctrl->key.speed_y = kb_vy_ramp->ramp_calc(spdslider, spdslider / 10);
                return rc_ctrl->key.speed_y;
            case Types::Kb_event::DOWN:
                rc_ctrl->key.speed_y = kb_vy_ramp->ramp_calc(-spdslider, spdslider / 10);
                return rc_ctrl->key.speed_y;
            case Types::Kb_event::LEFT:
                rc_ctrl->key.speed_x = kb_vx_ramp->ramp_calc(-spdslider, spdslider / 10);
                return rc_ctrl->key.speed_x;
            case Types::Kb_event::RIGHT:
                rc_ctrl->key.speed_x = kb_vx_ramp->ramp_calc(spdslider, spdslider / 10);
                return rc_ctrl->key.speed_x;
            case Types::Kb_event::SPIN_L:
                rc_ctrl->key.q = 0x1;
                rc_ctrl->key.f = 0x0;
                /* IEEE Not A Number.  */
                return NAN;
            case Types::Kb_event::SPIN_R:
                rc_ctrl->key.f = 0x1;
                rc_ctrl->key.q = 0x0;
                /* IEEE Not A Number.  */
                return NAN;
            case Types::Kb_event::STOP_X:;
                rc_ctrl->key.speed_x = kb_vx_ramp->ramp_calc(0, (fp32)Config::OPEN_LOOP_MAX_SPEED / 4);
                return rc_ctrl->key.speed_x;
            case Types::Kb_event::STOP_Y:;
                rc_ctrl->key.speed_y = kb_vy_ramp->ramp_calc(0, (fp32)Config::OPEN_LOOP_MAX_SPEED / 4);
                return rc_ctrl->key.speed_y;
        }
        return NAN;
    }
}  // namespace Input
