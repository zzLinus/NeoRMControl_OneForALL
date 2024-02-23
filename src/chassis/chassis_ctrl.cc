#include "chassis_ctrl.hpp"

#include <iostream>

namespace Chassis
{

    Chassis_ctrl::Chassis_ctrl()
    {
        // WARN:
        // first order low-pass filter  replace ramp function
        // 用一阶滤波代替斜波函数生成
        fof_x.num = Config::CHASSIS_ACCEL_X_NUM;
        fof_x.frame_period = Config::CHASSIS_CONTROL_TIME;
        fof_x.input = 0.0f;
        fof_x.out = 0.0f;

        fof_y.num = Config::CHASSIS_ACCEL_Y_NUM;
        fof_y.frame_period = Config::CHASSIS_CONTROL_TIME;
        fof_y.input = 0.0f;
        fof_y.out = 0.0f;

        // HACK:
        // 小陀螺缓启停斜波函数初始化
        // spin_ramp.frame_period = 0.1;
        // spin_ramp.max_value = Config::CHASSIS_WZ_SPIN;
        // spin_ramp.min_value = 0;
        // spin_ramp.input = 0.0f;
        // spin_ramp.out = 0.0f;

        // HACK:initialize angle PID
        // 初始化角度PID
        follow_angle_pid = new Pid::Pid_ctrller(
            Types::PID_POSITION,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_KP,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_KI,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_KD,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT,
            Config::CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT);

        no_follow_angle_pid = new Pid::Pid_ctrller(
            Types::PID_POSITION,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_KP,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_KI,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_KD,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_OUT,
            Config::CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_IOUT);

        // in beginning， chassis mode is raw
        // 底盘开机状态为原始
        mode = Types::CHSMODE_ZERO_FORCE;

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
            case Types::CHSMODE_FOLLOW_GIMBAL_YAW:
                this->chassis_relative_angle_set = 0.0f;
                return;
                // change to follow chassis yaw angle
                // 切入跟随底盘角度模式
            case Types::CHSMODE_FOLLOW_CHASSIS_YAW:
                // FIXME: yaw pitch row not working now
                this->chassis_yaw_set = this->chassis_yaw;
                return;
                // change to no follow angle
                // 切入不跟随云台模式
            case Types::CHSMODE_NO_FOLLOW_GIMBAL_YAW: this->chassis_yaw_set = this->chassis_yaw; return;
            default: return;
        }
    }

    void Chassis_ctrl::control_get_feedback()
    {
        for (auto& m : motors)
        {
            //      update motor speed, accel is differential of speed PID
            //      更新电机速度，加速度是速度的PID微分
            m->speed = Config::CHASSIS_MOTOR_RPM_TO_VECTOR_SEN * m->motor_measure->speed_rpm;
            m->accel = Config::CHASSIS_CONTROL_FREQUENCE * m->pid_ctrler->Dbuf[0];
        }

        // calculate vertical speed, horizontal speed ,rotation speed, left hand rule
        // 更新底盘纵向速度 x， 平移速度y，旋转速度wz，坐标系为右手系
        vx = (-motors[0]->speed + motors[1]->speed + motors[2]->speed - motors[3]->speed) *
             Config::MOTOR_SPEED_TO_CHASSIS_SPEED_VX;
        vy = (-motors[0]->speed - motors[1]->speed + motors[2]->speed + motors[3]->speed) *
             Config::MOTOR_SPEED_TO_CHASSIS_SPEED_VY;
        wz = (-motors[0]->speed - motors[1]->speed - motors[2]->speed - motors[3]->speed) *
             Config::MOTOR_SPEED_TO_CHASSIS_SPEED_WZ / Config::MOTOR_DISTANCE_TO_CENTER;

        // TODO: 计算底盘姿态角度, 如果底盘上有陀螺仪请更改这部分代码
        // calculate chassis euler angle, if chassis add a new gyro sensor,please change this code
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
        switch (this->mode)
        {
                // follow gimbal mode
                // 跟随云台模式
            case Types::CHSMODE_FOLLOW_GIMBAL_YAW:;
                //// TODO: implement FOLLOW_GIMBAL_YAW mode
                // fp32 sin_yaw = 0.0f, cos_yaw = 0.0f;
                //// rotate chassis direction, make sure vertial direction follow gimbal
                //// 旋转控制底盘速度方向，保证前进方向是云台方向，有利于运动平稳
                // sin_yaw = arm_sin_f32(-chassis_move_control->chassis_yaw_motor->relative_angle);
                // cos_yaw = arm_cos_f32(-chassis_move_control->chassis_yaw_motor->relative_angle);
                // chassis_move_control->vx_set = cos_yaw * vx_set + sin_yaw * vy_set;
                // chassis_move_control->vy_set = -sin_yaw * vx_set + cos_yaw * vy_set;
                //// set control relative angle  set-point
                //// 设置控制相对云台角度
                // chassis_move_control->chassis_relative_angle_set =
                //     rad_format(angle_set);  // 此模式下如果不开启摇摆，angle_set始终为0
                //                             // calculate ratation speed
                //                             // 计算旋转PID角速度
                // chassis_move_control->wz_set = -PID_calc(
                //     &chassis_move_control->chassis_angle_pid,
                //     chassis_move_control->chassis_yaw_motor->relative_angle,
                //     chassis_move_control->chassis_relative_angle_set);
                //// speed limit
                //// 速度限幅
                // chassis_move_control->vx_set = fp32_constrain(
                //     chassis_move_control->vx_set,
                //     chassis_move_control->vx_min_speed,
                //     chassis_move_control->vx_max_speed);
                // chassis_move_control->vy_set = fp32_constrain(
                //     chassis_move_control->vy_set,
                //     chassis_move_control->vy_min_speed,
                //     chassis_move_control->vy_max_speed);
            case Types::CHSMODE_FOLLOW_CHASSIS_YAW:;
                // fp32 delat_angle = 0.0f;
                //// set chassis yaw angle set-point
                //// 设置底盘控制的角度
                // chassis_move_control->chassis_yaw_set = rad_format(angle_set);
                // delat_angle = rad_format(chassis_move_control->chassis_yaw_set -
                // chassis_move_control->chassis_yaw);
                //// calculate rotation speed
                //// 计算旋转的角速度
                // chassis_move_control->wz_set = PID_calc(&chassis_move_control->chassis_angle_pid, 0.0f,
                // delat_angle);
                //// speed limit
                //// 速度限幅
                // chassis_move_control->vx_set =
                //     fp32_constrain(vx_set, chassis_move_control->vx_min_speed,
                //     chassis_move_control->vx_max_speed);
                // chassis_move_control->vy_set =
                //     fp32_constrain(vy_set, chassis_move_control->vy_min_speed,
                //     chassis_move_control->vy_max_speed);

            case Types::CHSMODE_SPIN:
            case Types::CHSMODE_NO_MOVE:
            case Types::CHSMODE_NO_FOLLOW_GIMBAL_YAW:;
                // fp32 sin_yaw = 0.0f, cos_yaw = 0.0f;
                //// 控制vx vy
                // sin_yaw = arm_sin_f32(-chassis_move_control->chassis_yaw_motor->relative_angle);
                // cos_yaw = arm_cos_f32(-chassis_move_control->chassis_yaw_motor->relative_angle);
                // chassis_move_control->vx_set = cos_yaw * vx_set + sin_yaw * vy_set;
                // chassis_move_control->vy_set = -sin_yaw * vx_set + cos_yaw * vy_set;
                // chassis_move_control->vx_set = fp32_constrain(
                //     chassis_move_control->vx_set,
                //     chassis_move_control->vx_min_speed,
                //     chassis_move_control->vx_max_speed);
                // chassis_move_control->vy_set = fp32_constrain(
                //     chassis_move_control->vy_set,
                //     chassis_move_control->vy_min_speed,
                //     chassis_move_control->vy_max_speed);
                //// 控制wz
                // if (chassis_behaviour_mode == CHASSIS_SPIN)  // 如果处在小陀螺模式
                //{
                //     chassis_move_control->wz_set = angle_set;
                // }
                // else  // 如果未在小陀螺模式 (angle_set来自遥控器,
                //       // chassis_no_follow_angle_pid用于以较慢的速度回正底盘)
                //{
                //     // 设置控制相对云台角度
                //     chassis_move_control->chassis_relative_angle_set = 0;
                //     // 计算旋转PID角速度
                //     chassis_move_control->wz_set =
                //         angle_set - PID_calc(
                //                         &chassis_move_control->chassis_no_follow_angle_pid,
                //                         chassis_move_control->chassis_yaw_motor->relative_angle,
                //                         chassis_move_control->chassis_relative_angle_set);
                // }

            case Types::CHSMODE_ZERO_FORCE:
                vx_set = 0;
                vx_set = 0;
                vx_set = 0;
                cmd_slow_set_vx.out = 0.0f;
                cmd_slow_set_vy.out = 0.0f;
                return;
            case Types::CHSMODE_OPEN:
                // in raw mode, set-point is sent to CAN bus
                // 在原始模式，设置值是发送到CAN总线
                vx_set = rc_ctrl->key.speed_x;
                vy_set = rc_ctrl->key.speed_y;

                wz_set = (rc_ctrl->key.q - rc_ctrl->key.f) * Config::OPEN_LOOP_MAX_SPEED;

                debug_info->vx = vx_set;
                debug_info->vy = vy_set;
                debug_info->wz = wz_set;
                cmd_slow_set_vx.out = 0.0f;
                cmd_slow_set_vy.out = 0.0f;
            default: return;
        }
    }

    void Chassis_ctrl::control_calc_pid()
    {
        fp32 wheel_speed[4];

        // mecanum wheel speed calculation
        // 麦轮运动分解
        // [3]  [0]
        //
        // [2]  [1]
        //
        wheel_speed[0] = vx_set - vy_set + wz_set;
        wheel_speed[1] = -vx_set - vy_set + wz_set;
        wheel_speed[2] = -vx_set + vy_set + wz_set;
        wheel_speed[3] = vx_set + vy_set + wz_set;

        switch (this->mode)
        {
            case Types::CHSMODE_ZERO_FORCE:;
            case Types::CHSMODE_OPEN:;
                for (uint8_t i = 0; i < 4; i++)
                {
                    motors[i]->give_current = (int16_t)(wheel_speed[i]);
                    // FIXME:
                    debug_info->wheel_speed[i] = wheel_speed[i];
                }
                return;

            default:
                // TODO: PID control
                // calculate the max speed in four wheels, limit the max speed
                // 计算轮子控制最大速度，并限制其最大速度
                // for (i = 0; i < 4; i++)
                //{
                //    chassis_move_control_loop->motor_chassis[i].speed_set = wheel_speed[i];
                //    temp = fabs(chassis_move_control_loop->motor_chassis[i].speed_set);
                //    if (max_vector < temp)
                //    {
                //        max_vector = temp;
                //    }
                //}
                // fp32 max_whell_speed =
                //    (chassis_behaviour_mode == CHASSIS_SPIN) ? MAX_WHEEL_SPIN_SPEED : MAX_WHEEL_SPEED;
                // if (max_vector > max_whell_speed)
                //{
                //    vector_rate = max_whell_speed / max_vector;
                //    for (i = 0; i < 4; i++)
                //    {
                //        chassis_move_control_loop->motor_chassis[i].speed_set *= vector_rate;
                //    }
                //}

                //// calculate pid
                //// 计算pid
                // for (i = 0; i < 4; i++)
                //{
                //     PID_calc(
                //         &chassis_move_control_loop->motor_speed_pid[i],
                //         chassis_move_control_loop->motor_chassis[i].speed,
                //         chassis_move_control_loop->motor_chassis[i].speed_set);
                // }

                //// 功率控制
                // chassis_power_control(chassis_move_control_loop);

                //// 赋值电流值
                // for (i = 0; i < 4; i++)
                //{
                //     chassis_move_control_loop->motor_chassis[i].give_current =
                //         (int16_t)(chassis_move_control_loop->motor_speed_pid[i].out);
                // }
                return;
        }
    }

    bool Chassis_ctrl::is_motor_online(void)
    {
        // TODO: implement motor on line check with candump here
        return true;
    }

    void Chassis_ctrl::set_motor_current()
    {
        uint64_t pkg = 0;
        int idx = 0;
        for (auto& m : motors)
        {
            pkg = pkg | (((uint64_t)m->give_current & 0xffff) << (16 * (3 - idx)));
            idx++;
        }
        debug_info->pkg = pkg;
        can_itrf->can_send(pkg);
    }

    void Chassis_ctrl::set_mode()
    {
        Types::mode_e last_mode = this->mode;
        bool is_spin = false;

        // TODO: set spin mode with RC controller && keyboard (0/2)
        if (!is_spin)
        {
            // if (this->spin_ramp.out <= this->spin_ramp.max_value / 1.5)  // 若小陀螺速度已减至足够小
            {
                // FIXME: didn't implement follow gimbal yaw mode yet
                // this->mode = Types::CHSMODE_NO_FOLLOW_GIMBAL_YAW;  // 则退出小陀螺模式
                this->mode = Types::CHSMODE_OPEN;  // 则退出小陀螺模式
                // this->spin_ramp.out = 0;           // 清空输出
            }
        }
        else
        {
            this->mode = Types::CHSMODE_SPIN;
        }
        //		last_state = chassis_behaviour_mode;

        // TODO: 当云台在某些模式下，像初始化， 底盘不动 遥控器离线，底盘不动
        // when gimbal in some mode, such as init mode, chassis must's move
        // if (gimbal_cmd_to_chassis_stop())
        if (0)
        {
            this->mode = Types::CHSMODE_NO_MOVE;
        }

        if (last_mode != this->mode)
            ecex_mode_switch();
    }

    void Chassis_ctrl::first_order_filter(fp32 intupt)
    {
    }

    void Chassis_ctrl::init(Input::Kb_ctrl* kb_ctrl)
    {
        // NOTE: init sub devices here
        can_itrf->init();

        // NOTE: get remote control point
        // 获取遥控器指针
        rc_ctrl = kb_ctrl->rc_ctrl;
        debug_info = kb_ctrl->debug;
        if (rc_ctrl != nullptr)
        {
            mode = Types::CHSMODE_OPEN;
        }
        else
        {
            mode = Types::CHSMODE_ZERO_FORCE;
        }
        // TODO: 获取陀螺仪姿态角指针
        // get gyro sensor euler angle point
        // cc->chassis_INS_angle = get_INS_angle_point();
        // TODO:获取云台电机数据指针
        // get gimbal motor data point
        // cc->chassis_yaw_motor = get_yaw_motor_point();
        // cc->chassis_pitch_motor = get_pitch_motor_point();
    }

}  // namespace Chassis
