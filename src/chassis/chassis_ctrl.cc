#include "chassis_ctrl.hpp"

namespace Chassis
{

    void Chassis_ctrl::ecex_mode_switch(void)
    {
        // TODO: implement modo shift behaviour here
        switch (this->mode)
        {
            case CHSMODE_FOLLOW_GIMBAL_YAW:;
            case CHSMODE_FOLLOW_CHASSIS_YAW:;
            case CHSMODE_NO_FOLLOW_GIMBAL_YAW:;
            default:;
        }
    }

    void Chassis_ctrl::control_get_error()
    {
    }

    void Chassis_ctrl::control_set_target()
    {
    }

    void Chassis_ctrl::control_calc_pid()
    {
    }

    bool Chassis_ctrl::is_motor_online(void)
    {
        // TODO: implement can interface here
        return true;
    }

    void Chassis_ctrl::set_motor_current()
    {
        // TODO: implement can interface here
        can_itrf->can_send();
    }

    void Chassis_ctrl::set_mode()
    {
        mode_e last_mode = this->mode;

        // TODO: mode shifting

        if (last_mode != this->mode)
            ecex_mode_switch();
    }

    Chassis_ctrl::Chassis_ctrl()
    {
        // FIXME:
        // first order low-pass filter  replace ramp function
        // 用一阶滤波代替斜波函数生成
        fof_x.num = CHASSIS_ACCEL_X_NUM;
        fof_x.frame_period = CHASSIS_CONTROL_TIME;
        fof_x.input = 0.0f;
        fof_x.out = 0.0f;

        // HACK: 键盘控制斜波函数初始化
        // 小陀螺缓启停斜波函数初始化
        fof_y.num = CHASSIS_ACCEL_Y_NUM;
        fof_y.frame_period = CHASSIS_CONTROL_TIME;
        fof_y.input = 0.0f;
        fof_y.out = 0.0f;

        kb_vx_ramp.frame_period = 0.1;
        kb_vx_ramp.max_value = NORMAL_MAX_CHASSIS_SPEED_X;
        kb_vx_ramp.min_value = -NORMAL_MAX_CHASSIS_SPEED_X;
        kb_vx_ramp.input = 0.0f;
        kb_vx_ramp.out = 0.0f;

        kb_vy_ramp.frame_period = 0.1;
        kb_vy_ramp.max_value = NORMAL_MAX_CHASSIS_SPEED_X;
        kb_vy_ramp.min_value = -NORMAL_MAX_CHASSIS_SPEED_X;
        kb_vy_ramp.input = 0.0f;
        kb_vy_ramp.out = 0.0f;

        kb_vy_ramp.frame_period = 0.1;
        kb_vy_ramp.max_value = CHASSIS_WZ_SPIN;
        kb_vy_ramp.min_value = 0;
        kb_vy_ramp.input = 0.0f;
        kb_vy_ramp.out = 0.0f;

        mode = CHSMODE_ZERO_FORCE;
        can_itrf = new Hardware::Can_interface();

        // TODO: move all pid configureation to a config file and load it in when init
        for (auto& m : this->motors)
        {
            m = new Hardware::Motor(1.0, 1.0, 1.0);
        }
    }

    Chassis_ctrl::~Chassis_ctrl()
    {
        delete can_itrf;
        for (auto& m : this->motors)
        {
            delete m;
        }
    }

    void Chassis_ctrl::first_order_filter(fp32 intupt)
    {
    }

    void Chassis_ctrl::init()
    {
		can_itrf->init();
    }

}  // namespace Chassis
