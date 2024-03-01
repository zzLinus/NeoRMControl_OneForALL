#include "kb_ctrl.hpp"

namespace Input
{
    Kb_ctrl::Kb_ctrl(std::shared_ptr<Robot::Robot_set> robot_set) {
        // HACK:键盘控制斜波函数初始化
        p_robot_set = robot_set;
        kb_vx_ramp = new Ramp::Ramp_ctrller();
        kb_vy_ramp = new Ramp::Ramp_ctrller();
    }

    Kb_ctrl::~Kb_ctrl() {
        delete kb_vx_ramp;
        delete kb_vy_ramp;
    }

    fp32 Kb_ctrl::event_handler(Types::Kb_event event, fp32 spdslider) {
        switch (event) {
            case Types::Kb_event::UP:
                p_robot_set->vy_set = kb_vy_ramp->ramp_calc(spdslider, spdslider / 30);
                return p_robot_set->vy_set;
            case Types::Kb_event::DOWN:
                p_robot_set->vy_set = kb_vy_ramp->ramp_calc(-spdslider, spdslider / 30);
                return p_robot_set->vy_set;
            case Types::Kb_event::LEFT:
                p_robot_set->vx_set = kb_vx_ramp->ramp_calc(spdslider, spdslider / 30);
                return p_robot_set->vx_set;
            case Types::Kb_event::RIGHT:
                p_robot_set->vx_set = kb_vx_ramp->ramp_calc(-spdslider, spdslider / 30);
                return p_robot_set->vx_set;
            case Types::Kb_event::SPIN_L:
                return 0;
            case Types::Kb_event::SPIN_R:
                return 0;
            case Types::Kb_event::STOP_X:;
                p_robot_set->vx_set = kb_vx_ramp->ramp_calc(spdslider, 2.5/ 20);
                return p_robot_set->vx_set;
            case Types::Kb_event::STOP_Y:;
                p_robot_set->vy_set = kb_vy_ramp->ramp_calc(spdslider, 2.5/ 20);
                return p_robot_set->vy_set;
        }
        return NAN;
    }
}  // namespace Input
