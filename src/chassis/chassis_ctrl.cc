#include "chassis_ctrl.hpp"

namespace Chassis
{

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

        // max and min speed
        // 最大 最小速度
        vx_max_speed = Config::NORMAL_MAX_CHASSIS_SPEED_X;
        vx_min_speed = -Config::NORMAL_MAX_CHASSIS_SPEED_X;

        vy_max_speed = Config::NORMAL_MAX_CHASSIS_SPEED_Y;
        vy_min_speed = -Config::NORMAL_MAX_CHASSIS_SPEED_Y;

        kb_vx_ramp.frame_period = 0.1;
        kb_vx_ramp.max_value = this->vx_max_speed;
        kb_vx_ramp.min_value = -this->vx_max_speed;
        kb_vx_ramp.input = 0.0f;
        kb_vx_ramp.out = 0.0f;

        kb_vy_ramp.frame_period = 0.1;
        kb_vy_ramp.max_value = this->vy_max_speed;
        kb_vy_ramp.min_value = -this->vy_max_speed;
        kb_vy_ramp.input = 0.0f;
        kb_vy_ramp.out = 0.0f;

        spin_ramp.frame_period = 0.1;
        spin_ramp.max_value = Config::CHASSIS_WZ_SPIN;
        spin_ramp.min_value = 0;
        spin_ramp.input = 0.0f;
        spin_ramp.out = 0.0f;

        // HACK:initialize angle PID
        // 初始化角度PID
        follow_angle_pid = new Pid::Pid_controller(
            Config::PID_POSITION,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_KP,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_KI,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_KD,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT);

        no_follow_angle_pid = new Pid::Pid_controller(
            Config::PID_POSITION,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_KP,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_KI,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_KD,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_OUT,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_IOUT);

        // in beginning， chassis mode is raw
        // 底盘开机状态为原始
        mode = CHSMODE_ZERO_FORCE;

        can_itrf = new Hardware::Can_interface();

        for (auto& m : this->motors)
        {
            m = new Hardware::Motor();
        }
    }

    Chassis_ctrl::~Chassis_ctrl()
    {
        delete can_itrf;
        delete follow_angle_pid;
        delete no_follow_angle_pid;
        for (auto& m : this->motors)
        {
            delete m;
        }
    }

    void Chassis_ctrl::ecex_mode_switch(void)
    {
        // TODO: implement modo shift behaviour here
        switch (this->mode)
        {
                // change to follow gimbal angle mode
                // 切入跟随云台模式
            case CHSMODE_FOLLOW_GIMBAL_YAW:
                this->chassis_relative_angle_set = 0.0f;
                return;
                // change to follow chassis yaw angle
                // 切入跟随底盘角度模式
            case CHSMODE_FOLLOW_CHASSIS_YAW:
                this->chassis_yaw_set = this->chassis_yaw;
                return;
                // change to no follow angle
                // 切入不跟随云台模式
            case CHSMODE_NO_FOLLOW_GIMBAL_YAW: this->chassis_yaw_set = this->chassis_yaw; return;
            default: return;
        }
    }

    void Chassis_ctrl::control_get_feedback()
    {
        // for (uint8_t i = 0; i < 4; i++)
        //{
        //     // update motor speed, accel is differential of speed PID
        //     // 更新电机速度，加速度是速度的PID微分
        //     chassis_move_update->motor_chassis[i].speed =
        //         CHASSIS_MOTOR_RPM_TO_VECTOR_SEN *
        //         chassis_move_update->motor_chassis[i].chassis_motor_measure->speed_rpm;
        //     chassis_move_update->motor_chassis[i].accel =
        //         chassis_move_update->motor_speed_pid[i].Dbuf[0] * CHASSIS_CONTROL_FREQUENCE;
        // }
        for (auto& m : motors)
        {
        }

        //// calculate vertical speed, horizontal speed ,rotation speed, left hand rule
        //// 更新底盘纵向速度 x， 平移速度y，旋转速度wz，坐标系为右手系
        // chassis_move_update->vx =
        //     (-chassis_move_update->motor_chassis[0].speed + chassis_move_update->motor_chassis[1].speed +
        //      chassis_move_update->motor_chassis[2].speed - chassis_move_update->motor_chassis[3].speed) *
        //     MOTOR_SPEED_TO_CHASSIS_SPEED_VX;
        // chassis_move_update->vy =
        //     (-chassis_move_update->motor_chassis[0].speed - chassis_move_update->motor_chassis[1].speed +
        //      chassis_move_update->motor_chassis[2].speed + chassis_move_update->motor_chassis[3].speed) *
        //     MOTOR_SPEED_TO_CHASSIS_SPEED_VY;
        // chassis_move_update->wz =
        //     (-chassis_move_update->motor_chassis[0].speed - chassis_move_update->motor_chassis[1].speed -
        //      chassis_move_update->motor_chassis[2].speed - chassis_move_update->motor_chassis[3].speed) *
        //     MOTOR_SPEED_TO_CHASSIS_SPEED_WZ / MOTOR_DISTANCE_TO_CENTER;

        //// calculate chassis euler angle, if chassis add a new gyro sensor,please change this code
        //// 计算底盘姿态角度, 如果底盘上有陀螺仪请更改这部分代码
        // chassis_move_update->chassis_yaw = rad_format(
        //     *(chassis_move_update->chassis_INS_angle + INS_YAW_ADDRESS_OFFSET) -
        //     chassis_move_update->chassis_yaw_motor->relative_angle);
        // chassis_move_update->chassis_pitch = rad_format(
        //     *(chassis_move_update->chassis_INS_angle + INS_PITCH_ADDRESS_OFFSET) -
        //     chassis_move_update->chassis_pitch_motor->relative_angle);
        // chassis_move_update->chassis_roll = *(chassis_move_update->chassis_INS_angle + INS_ROLL_ADDRESS_OFFSET);
    }

    void Chassis_ctrl::control_set_target()
    {
    }

    void Chassis_ctrl::control_calc_pid()
    {
    }

    bool Chassis_ctrl::is_motor_online(void)
    {
        // TODO: implement motor on line check with candump here
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
        bool is_spin = true;

        // TODO: mode shifting
        // remote control  set chassis behaviour mode
        // 遥控器设置模式
        //
        // TODO: set spin mode with RC controller && keyboard (0/2)

        if (!is_spin)
        {
            if (this->spin_ramp.out <= this->spin_ramp.max_value / 1.5)  // 若小陀螺速度已减至足够小
            {
                this->mode = CHSMODE_NO_FOLLOW_GIMBAL_YAW;  // 则退出小陀螺模式
                this->spin_ramp.out = 0;                    // 清空输出
            }
        }
        else
        {
            this->mode = CHSMODE_SPIN;
        }
        //		last_state = chassis_behaviour_mode;

        // when gimbal in some mode, such as init mode, chassis must's move
        // TODO: 当云台在某些模式下，像初始化， 底盘不动
        // if (gimbal_cmd_to_chassis_stop())
        if (0)
        {
            this->mode = CHSMODE_NO_MOVE;
        }

        // add your own logic to enter the new mode
        // 添加自己的逻辑判断进入新模式

        if (last_mode != this->mode)
            ecex_mode_switch();
    }

    void Chassis_ctrl::first_order_filter(fp32 intupt)
    {
    }

    void Chassis_ctrl::init()
    {
        // NOTE: init sub devices here
        can_itrf->init();
        // TODO: get remote control point
        // 获取遥控器指针
        // cc->chassis_RC = get_remote_control_point();
        // get gyro sensor euler angle point
        // TODO: 获取陀螺仪姿态角指针
        // cc->chassis_INS_angle = get_INS_angle_point();
        // get gimbal motor data point
        // TODO:获取云台电机数据指针
        // cc->chassis_yaw_motor = get_yaw_motor_point();
        // cc->chassis_pitch_motor = get_pitch_motor_point();
    }

}  // namespace Chassis
