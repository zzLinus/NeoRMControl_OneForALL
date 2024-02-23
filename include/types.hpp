#ifndef __TYPES__
#define __TYPES__
#include <linux/can.h>

#include <cstdint>

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

    const fp32 CHASSIS_WZ_SPIN = 7;

    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VX = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VY = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_WZ = 0.25f;
    const fp32 MOTOR_DISTANCE_TO_CENTER = 0.2f;

    // m3508 rmp change to chassis speed,
    // m3508转化成底盘速度(m/s)的比例，
    const fp32 CHASSIS_MOTOR_RPM_TO_VECTOR_SEN = 0.000415809748903494517209f;
    const fp32 CHASSIS_CONTROL_FREQUENCE = 500.0f;

    // NOTE: chassis task control time 0.002s
    // 底盘任务控制间隔 0.002s
    const fp32 CHASSIS_CONTROL_TIME = 0.002f;
    const fp32 CHASSIS_ACCEL_X_NUM = 0.1666666667f;
    const fp32 CHASSIS_ACCEL_Y_NUM = 0.3333333333f;

    //
    const fp32 RAMP_KEY_ADD_VX = 0.06f;
    const fp32 RAMP_KEY_ADD_VY = 0.06f;
    // 小陀螺缓启停的增量
    // 小陀螺缓启停的增量
    const fp32 RAMP_SPIN_INC = 0.06f;
    const fp32 RAMP_SPIN_DEC = 0.06f;

    const int32_t OPEN_LOOP_MAX_SPEED = 0x03ff;
}  // namespace Config

namespace Types
{

    typedef struct
    {
        fp32 input;         // 输入数据
        fp32 out;           // 滤波输出的数据
        fp32 num[1];        // 滤波参数
        fp32 frame_period;  // 滤波的时间间隔 单位 s
    } first_order_filter_type_t;

    enum PID_MODE
    {
        PID_POSITION = 0,
        PID_DELTA
    };

    typedef struct
    {
        fp32 vx;
        fp32 vy;
        fp32 wz;
        fp32 wheel_speed[4];
        can_frame can_f;
        uint64_t pkg;
    } debug_info_t;

    typedef struct
    {
        struct
        {
            int16_t ch[5];
            char s[2];
        } rc;
        struct
        {
            int16_t x;
            int16_t y;
            int16_t z;
            uint8_t press_l;
            uint8_t press_r;
        } mouse;
        struct
        {
            bool q;  //  spin clock wise
            bool f;  //  spin counter clock wise
            fp32 speed_x;
            fp32 speed_y;
            int16_t v;
        } key;

    } RC_ctrl_t;

    typedef enum
    {
        CHSMODE_ZERO_FORCE = 0x1 << 0,  // chassis will be like no power,底盘无力, 跟没上电那样
        CHSMODE_NO_MOVE = 0x1 << 1,     // chassis will be stop,底盘保持不动
        CHSMODE_FOLLOW_GIMBAL_YAW = 0x1 << 2,
        CHSMODE_FOLLOW_CHASSIS_YAW = 0x1 << 3,
        // speed 底盘不跟随角度，角度是开环的，但轮子是有速度环
        // 底盘不跟随云台，但左右推左摇杆时底盘也会向左向右转
        CHSMODE_NO_FOLLOW_GIMBAL_YAW = 0x1 << 4,
        CHSMODE_OPEN = 0x1 << 5,  //  遥控器的值乘以比例成电流值 直接发送到can总线上
        CHSMODE_SPIN = 0x1 << 6   // 新增：小陀螺模式
    } mode_e;

    typedef struct
    {
        fp32 input;         // 输入数据
        fp32 out;           // 滤波输出的数据
        fp32 num;           // 滤波参数
        fp32 frame_period;  // 滤波的时间间隔 单位 s
    } fof_t;

    typedef struct
    {
    } ramp_t;
}  // namespace Types

#endif
