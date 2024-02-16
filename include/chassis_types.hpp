
#ifndef __CHASSIS_TYPE__
#define __CHASSIS_TYPE__
// NOTE: chassis mode

#include "types.hpp"
namespace Chassis
{

// FIXME:chassis task control time 0.002s
// 底盘任务控制间隔 0.002s
#define CHASSIS_CONTROL_TIME            0.002f
#define CHASSIS_ACCEL_X_NUM             0.1666666667f
#define CHASSIS_ACCEL_Y_NUM             0.3333333333f
#define NORMAL_MAX_CHASSIS_SPEED_X      2.0f  // origin: 2.0f
#define NORMAL_MAX_CHASSIS_SPIN_SPEED_X 3.0f  // origin: 2.0f
#define NORMAL_MAX_CHASSIS_SPEED_Y      2.0f  // origin: 1.5f
#define NORMAL_MAX_CHASSIS_SPIN_SPEED_Y 3.0f  // origin: 1.5f
#define CHASSIS_WZ_SPIN                 7

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
        fp32 input;         // 输入数据
        fp32 out;           // 滤波输出的数据
        fp32 num;           // 滤波参数
        fp32 frame_period;  // 滤波的时间间隔 单位 s
    } fof_t;

    typedef struct
    {
        fp32 input;         // 输入数据
        fp32 out;           // 输出数据
        fp32 min_value;     // 限幅最小值
        fp32 max_value;     // 限幅最大值
        fp32 frame_period;  // 时间间隔
    } ramp_t;

}  // namespace Chassis

#endif
