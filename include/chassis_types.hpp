
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
#define CHASSIS_WZ_SPIN 7

    typedef enum
    {
        CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW,   // chassis will follow yaw gimbal motor
        CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW,  // chassis will have yaw angle(chassis_yaw)
                                            // close-looped
        CHASSIS_VECTOR_NO_FOLLOW_YAW,       // chassis will have rotation speed control.
        CHASSIS_VECTOR_RAW,                 // control-current will be sent to CAN bus derectly.

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
        fp32 input;         // 输入数据
        fp32 out;           // 滤波输出的数据
        fp32 num;           // 滤波参数
        fp32 frame_period;  // 滤波的时间间隔 单位 s
    } first_order_filter_type_t;

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
