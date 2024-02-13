#ifndef __TYPES__
#define __TYPES__

// NOTE: defines and type defines
typedef signed char int8_t;
typedef signed short int int16_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

#define OK    true
#define ERROR false

enum PID_MODE
{
    PID_POSITION = 0,
    PID_DELTA
};


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
        uint16_t v;
    } key;

} RC_ctrl_t;

typedef struct
{
    fp32 input;         // 输入数据
    fp32 out;           // 滤波输出的数据
    fp32 num[1];        // 滤波参数
    fp32 frame_period;  // 滤波的时间间隔 单位 s
} first_order_filter_type_t;

#endif
