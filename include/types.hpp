#ifndef __TYPES__
#define __TYPES__
#include "string"

// NOTE: defines and type defines
typedef signed char int8_t;
typedef signed short int int16_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

namespace Status
{
    const bool OK = true;
    const bool ERROR = false;
}  // namespace Status

namespace Config
{
    const char CAN_CHANNEL[] = "can0";

    // NOTE: PID CONFIG
    const fp32 M3505_MOTOR_SPEED_PID_KP = 15000.0f;  // 15000.0f
    const fp32 M3505_MOTOR_SPEED_PID_KI = 10.0f;
    const fp32 M3505_MOTOR_SPEED_PID_KD = 0.0f;
    // 3508 MAX_MOTOR_CAN_CURRENT 14000.0f
    const fp32 M3505_MOTOR_SPEED_PID_MAX_OUT = 14000.0f;
    const fp32 M3505_MOTOR_SPEED_PID_MAX_IOUT = 2000.0f;

    // chassis follow angle PID
    // 底盘旋转跟随PID
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_KP = 20.0f;  // 原始40.0f
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_KI = 0.0f;
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_KD = 100.0f;  // 原始0.0f
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT = 6.0f;
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT = 0.2f;

    // NOTE: chassis
    // chassis no follow angle PID
    // 底盘旋转不跟随PID
    // NOTE: const static fp32 chassis_no_follow_yaw_pid[3] = {4.5, 0.0002, 0}; ZHANG
    //       const static fp32 chassis_no_follow_yaw_pid[3] = {3.5, 0.0002, 0}; HUAN
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_KP = 4.5f;  // 原始40.0f
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_KI = 0.0002f;
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_KD = 0.0f;  // 原始0.0f
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_OUT = 6.0f;
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_IOUT = 0.2f;

    const fp32 NORMAL_MAX_CHASSIS_SPEED_X = 2.0f;       // origin: 2.0f
    const fp32 NORMAL_MAX_CHASSIS_SPIN_SPEED_X = 3.0f;  // origin: 2.0f
    const fp32 NORMAL_MAX_CHASSIS_SPEED_Y = 2.0f;       // origin: 1.5f
    const fp32 NORMAL_MAX_CHASSIS_SPIN_SPEED_Y = 3.0f;  // origin: 1.5f
                                                        //
    const fp32 CHASSIS_WZ_SPIN = 7;

    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VX = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VY = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_WZ = 0.25f;
    const fp32 MOTOR_DISTANCE_TO_CENTER = 0.2f;

    //
    const fp32 RAMP_KEY_ADD_VX = 0.06f;
    const fp32 RAMP_KEY_ADD_VY = 0.06f;
    // 小陀螺缓启停的增量
    // 小陀螺缓启停的增量
    const fp32 RAMP_SPIN_INC = 0.06f;
    const fp32 RAMP_SPIN_DEC = 0.06f;

    enum PID_MODE
    {
        PID_POSITION = 0,
        PID_DELTA
    };

}  // namespace Config


typedef struct
{
    fp32 input;         // 输入数据
    fp32 out;           // 滤波输出的数据
    fp32 num[1];        // 滤波参数
    fp32 frame_period;  // 滤波的时间间隔 单位 s
} first_order_filter_type_t;

#endif
