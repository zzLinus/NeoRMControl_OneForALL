#ifndef __CHASSIS_CTRL__
#define __CHASSIS_CTRL__

#include "can.hpp"
#include "chassis_types.hpp"
#include "motor.hpp"
#include "pid_controller.hpp"

namespace Chassis
{
    class Chassis_ctrl
    {
       public:
        Chassis_ctrl();
        ~Chassis_ctrl();
        void init(void);
        void first_order_filter(fp32 intupt);
        void set_mode(void);
        bool is_motor_online(void);
        void set_motor_current();
        void control_get_feedback();
        void control_set_target();
        void control_calc_pid();

       private:
        void ecex_mode_switch(void);

       public:
        fp32 vx;  // chassis vertical speed, positive means forward,unit m/s. 底盘速度 前进方向 前为正，单位 m/s
        fp32 vy;  // chassis horizontal speed, positive means letf,unit m/s.底盘速度 左右方向 左为正  单位 m/s
        fp32 wz;  // chassis rotation speed, positive means counterclockwise,unit rad/s.底盘旋转角速度，逆时针为正
                  // 单位 rad/s
        Hardware::Motor *motors[4];
        Types::fof_t fof_x;  // HACK: better come up with a better name
        Types::fof_t fof_y;  // HACK: better come up with a better name

        Types::ramp_t kb_vx_ramp;  // 用于键盘控制的斜波函数
        Types::ramp_t kb_vy_ramp;  // 用于键盘控制的斜波函数
        Types::ramp_t spin_ramp;

        Types::mode_e mode;

        //
        // HACK: drop motor_speed_pid motor_chassis, put all these mess in to motor class
        // chassis_motor_t motor_chassis[4];  // chassis motor data.底盘电机数据
        // pid_type_def motor_speed_pid[4];   // motor speed PID.底盘电机速度pid

        Pid::Pid_controller *follow_angle_pid;     // follow angle PID.底盘跟随角度pid
        Pid::Pid_controller *no_follow_angle_pid;  // 底盘不跟随角度pid  added by 片哥

        Types::fof_t cmd_slow_set_vx;  // use first order filter to slow set-point.使用一阶低通滤波减缓设定值
        Types::fof_t cmd_slow_set_vy;  // use first order filter to slow set-point.使用一阶低通滤波减缓设定值

        // TODO: remote controller && imu && gimbal related
        const Types::RC_ctrl_t * rc_ctrl;  // 底盘使用的遥控器指针, the point to remote control
        // const gimbal_motor_t *chassis_yaw_motor;   //will use the relative angle of yaw gimbal motor to calculate
        // the euler angle.底盘使用到yaw云台电机的相对角度来计算底盘的欧拉角. const gimbal_motor_t
        // *chassis_pitch_motor; //will use the relative angle of pitch gimbal motor to calculate the euler
        // angle.底盘使用到pitch云台电机的相对角度来计算底盘的欧拉角
        // const fp32 *chassis_INS_angle;  // the point to
        // the euler angle of gyro sensor.获取陀螺仪解算出的欧拉角指针

        fp32 vx_set;  // chassis set vertical speed,positive means forward,unit m/s.底盘设定速度 前进方向
                      // 前为正，单位 m/s
        fp32 vy_set;  // chassis set horizontal speed,positive means left,unit m/s.底盘设定速度 左右方向
                      // 左为正，单位 m/s
        fp32 wz_set;  // chassis set rotation speed,positive means counterclockwise,unit
                      // rad/s.底盘设定旋转角速度，逆时针为正 单位 rad/s
        fp32 chassis_relative_angle;  // the relative angle between chassis and gimbal.底盘与云台的相对角度，单位
                                      // rad
        fp32 chassis_relative_angle_set;  // the set relative angle.设置相对云台控制角度
        fp32 chassis_yaw_set;

        fp32 chassis_yaw;  // the yaw angle calculated by gyro sensor and gimbal motor.陀螺仪和云台电机叠加的yaw角度
        fp32 chassis_pitch;  // the pitch angle calculated by gyro sensor and gimbal
                             // motor.陀螺仪和云台电机叠加的pitch角度
        fp32 chassis_roll;   // the roll angle calculated by gyro sensor and gimbal
                             // motor.陀螺仪和云台电机叠加的roll角度
       private:
        Hardware::Can_interface *can_itrf;
    };
}  // namespace Chassis

#endif
