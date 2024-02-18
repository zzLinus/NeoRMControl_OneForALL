
#ifndef __CHASSIS_TYPE__
#define __CHASSIS_TYPE__
// NOTE: chassis mode

#include "types.hpp"

namespace Chassis
{

// FIXME:chassis task control time 0.002s
// 底盘任务控制间隔 0.002s
#define CHASSIS_CONTROL_TIME 0.002f
#define CHASSIS_ACCEL_X_NUM  0.1666666667f
#define CHASSIS_ACCEL_Y_NUM  0.3333333333f
    namespace Types
    {
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
                bool w;  //  ⬆️
                bool r;  //  ⬇️
                bool a;  //  ⬅️
                bool s;  //  ➡️
                bool q;  //  spin clock wise
                bool f;  //  spin counter clock wise
                uint16_t speed;
                uint16_t v;
            } key;

        } RC_ctrl_t;

        // m3508 rmp change to chassis speed,
        // m3508转化成底盘速度(m/s)的比例，
        const fp32 CHASSIS_MOTOR_RPM_TO_VECTOR_SEN = 0.000415809748903494517209f;
        const fp32 CHASSIS_CONTROL_FREQUENCE = 500.0f;

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
            fp32 input;         // 输入数据
            fp32 out;           // 输出数据
            fp32 min_value;     // 限幅最小值
            fp32 max_value;     // 限幅最大值
            fp32 frame_period;  // 时间间隔
        } ramp_t;
    }  // namespace Types

}  // namespace Chassis

#endif
