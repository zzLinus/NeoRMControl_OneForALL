#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include <memory>
#include <vector>

#include "robot.hpp"
#include "hardware.hpp"
#include "can.hpp"
#include "motor.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace Chassis
{
    class Chassis
    {
       public:
        Chassis();
        ~Chassis() = default;
        void update_data();
        void init(const std::shared_ptr<Robot::Robot_set> &robot);
        void decomposition_speed();
        [[noreturn]] void task();

       public:
        fp32 vx_set = 0.f;
        // chassis set vertical speed,positive means forward,unit m/s.底盘设定速度 前进方向
        // 前为正，单位 m/s
        fp32 vy_set = 0.f;
        // chassis set horizontal speed,positive means left,unit m/s.底盘设定速度 左右方向
        // 左为正，单位 m/s
        fp32 wz_set = 0.f;

//        std::shared_ptr<Hardware::Can_interface> can_itrf;
       private:
        // chassis set rotation speed,positive means counterclockwise,unit
        // rad/s.底盘设定旋转角速度，逆时针为正 单位 rad/s
        fp32 vx = 0.f;
        // chassis vertical speed, positive means forward,unit m/s. 底盘速度 前进方向 前为正，单位 m/s
        fp32 vy = 0.f;
        // chassis horizontal speed, positive means letf,unit m/s.底盘速度 左右方向 左为正  单位 m/s
        fp32 wz = 0.f;
        // chassis rotation speed, positive means counterclockwise,unit rad/s.底盘旋转角速度，逆时针为正
        fp32 wheel_speed[4] = {};

        fp32 max_wheel_speed = 2.5f;
        Pid::Pid_rad chassis_angle_pid;

        std::vector<Hardware::Motor> motors;
        std::shared_ptr<Robot::Robot_set> robot_set;
    };
}  // namespace Chassis

#endif
