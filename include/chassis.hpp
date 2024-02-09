#ifndef __CHASSIS__
#define __CHASSIS__

#include "types.hpp"
#include "pid.h"

namespace Chassis {

// NOTE: chassis mode
typedef enum {
  CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW,  // chassis will follow yaw gimbal motor
  CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW, // chassis will have yaw angle(chassis_yaw)
                                     // close-looped
  CHASSIS_VECTOR_NO_FOLLOW_YAW, // chassis will have rotation speed control.
  CHASSIS_VECTOR_RAW, // control-current will be sent to CAN bus derectly.

} chassis_mode_e;

typedef struct
{
	// FIXME: can bus api related
    // const motor_measure_t *chassis_motor_measure;
	// const motor_measure_t *chassis_motor_measure;
    fp32 accel;
    fp32 speed;
    fp32 speed_set;
    int16_t give_current;
} chassis_motor_t;

typedef struct
{
    fp32 input;        //输入数据
    fp32 out;          //滤波输出的数据
    fp32 num[1];       //滤波参数
    fp32 frame_period; //滤波的时间间隔 单位 s
} first_order_filter_type_t;

typedef struct
{
    fp32 input;        //输入数据
    fp32 out;          //输出数据
    fp32 min_value;    //限幅最小值
    fp32 max_value;    //限幅最大值
    fp32 frame_period; //时间间隔
} ramp_function_source_t;

class Chassis_info {
private:
public:
	// FIXME: remote controller && imu && gimbal related
	// const RC_ctrl_t *chassis_RC;               //底盘使用的遥控器指针, the point to remote control
    // const gimbal_motor_t *chassis_yaw_motor;   //will use the relative angle of yaw gimbal motor to calculate the euler angle.底盘使用到yaw云台电机的相对角度来计算底盘的欧拉角.
    // const gimbal_motor_t *chassis_pitch_motor; //will use the relative angle of pitch gimbal motor to calculate the euler angle.底盘使用到pitch云台电机的相对角度来计算底盘的欧拉角
    // const fp32 *chassis_INS_angle;             //the point to the euler angle of gyro sensor.获取陀螺仪解算出的欧拉角指针
	
	Chassis_info();
	~Chassis_info();
	
    chassis_mode_e chassis_mode;               //state machine. 底盘控制状态机
    chassis_mode_e last_chassis_mode;          //last state machine.底盘上次控制状态机
	chassis_motor_t motor_chassis[4];          //chassis motor data.底盘电机数据
    pid_type_def motor_speed_pid[4];             //motor speed PID.底盘电机速度pid
    pid_type_def chassis_angle_pid;              //follow angle PID.底盘跟随角度pid

    pid_type_def chassis_no_follow_angle_pid;              //底盘不跟随角度pid  added by 片哥

    first_order_filter_type_t chassis_cmd_slow_set_vx;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
    first_order_filter_type_t chassis_cmd_slow_set_vy;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值

    fp32 vx;                          //chassis vertical speed, positive means forward,unit m/s. 底盘速度 前进方向 前为正，单位 m/s
    fp32 vy;                          //chassis horizontal speed, positive means letf,unit m/s.底盘速度 左右方向 左为正  单位 m/s
    fp32 wz;                          //chassis rotation speed, positive means counterclockwise,unit rad/s.底盘旋转角速度，逆时针为正 单位 rad/s
    fp32 vx_set;                      //chassis set vertical speed,positive means forward,unit m/s.底盘设定速度 前进方向 前为正，单位 m/s
    fp32 vy_set;                      //chassis set horizontal speed,positive means left,unit m/s.底盘设定速度 左右方向 左为正，单位 m/s
    fp32 wz_set;                      //chassis set rotation speed,positive means counterclockwise,unit rad/s.底盘设定旋转角速度，逆时针为正 单位 rad/s
    fp32 chassis_relative_angle;      //the relative angle between chassis and gimbal.底盘与云台的相对角度，单位 rad
    fp32 chassis_relative_angle_set;  //the set relative angle.设置相对云台控制角度
    fp32 chassis_yaw_set;

    fp32 vx_max_speed;  //max forward speed, unit m/s.前进方向最大速度 单位m/s
    fp32 vx_min_speed;  //max backward speed, unit m/s.后退方向最大速度 单位m/s
    fp32 vy_max_speed;  //max letf speed, unit m/s.左方向最大速度 单位m/s
    fp32 vy_min_speed;  //max right speed, unit m/s.右方向最大速度 单位m/s
    fp32 chassis_yaw;   //the yaw angle calculated by gyro sensor and gimbal motor.陀螺仪和云台电机叠加的yaw角度
    fp32 chassis_pitch; //the pitch angle calculated by gyro sensor and gimbal motor.陀螺仪和云台电机叠加的pitch角度
    fp32 chassis_roll;  //the roll angle calculated by gyro sensor and gimbal motor.陀螺仪和云台电机叠加的roll角度

	// TODO: keyboard controller logic
    //ramp_function_source_t key_vx_ramp;  //用于键盘控制的斜波函数
    //ramp_function_source_t key_vy_ramp;  //用于键盘控制的斜波函数

    ramp_function_source_t chassis_spin_ramp;  //用于小陀螺缓启停的斜波函数
    fp32 chassis_spin_ramp_add;  //小陀螺缓启停的增量

};

} // namespace chassis

#endif
