#ifndef __ROBOT__
#define __ROBOT__
#include "can.hpp"
#include "hardware_manager.hpp"
#include "memory"
#include "serial_interface.hpp"
#include "types.hpp"

namespace Robot
{
    struct Robot_set
    {
        uint8_t heaedr;
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

}  // namespace Robot

#endif
