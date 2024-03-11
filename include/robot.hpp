#ifndef __ROBOT__
#define __ROBOT__
#include "can.hpp"
#include "hardware_manager.hpp"
#include "memory"
#include "serial_interface.hpp"
#include "types.hpp"

namespace Robot
{
    // robot set header = 0xEA;
    struct Robot_set
    {
        uint8_t header;
        /** chassis_control **/
        fp32 vx_set = 0.f;
        fp32 vy_set = 0.f;
        fp32 wz_set = 0.f;

        /** gimbal_control **/
        fp32 yaw_set = 0.f;
        fp32 pitch_set = 0.f;

        /** IMU **/
        fp32 ins_yaw = 0.f;
        fp32 ins_pitch = 0.f;
        fp32 ins_roll = 0.f;
        fp32 ins_yaw_v = 0.f;
        fp32 ins_pitch_v = 0.f;
        fp32 ins_roll_v = 0.f;

        /** other **/
        fp32 yaw_relative = 0.f;
        fp32 pitch_relative = 0.f;
        Types::ROBOT_MODE mode = Types::ROBOT_MODE::ROBOT_NO_FORCE;
    } __attribute__((packed));

    // send gimbal package header = 0x5A;
    struct SendGimbalPacket
    {
        /*      自瞄部分     */
        uint8_t header;
        bool tracking;
        uint8_t id;
        uint8_t armors_num;
        uint8_t reserved;
        float x;
        float y;
        float z;
        float yaw;
        float vx;
        float vy;
        float vz;
        float v_yaw;
        float r1;
        float r2;
        float dz;
        /*  决策部分   */

    } __attribute__((packed));

    // send twist package heaedr = 0x6A;
    struct SendTwistPacket
    {
        /*       导航部分       */
        uint8_t header;
        // 线速度 m/s
        float linear_vx;
        float linear_vy;
        float linear_vz;
        // 旋转角速度 rad/s
        float angular_vx;
        float angular_vy;
        float angluar_vz;
        // 欧拉角
        float twist_v_yaw;
        float twist_v_roll;
        float twist_v_pitch;

    } __attribute__((packed));

}  // namespace Robot

#endif
