#pragma once
#include "chassis_config.hpp"
#include "robot.hpp"
#include "motor.hpp"

#include <memory>

namespace Chassis
{
    class Mecanum : public ChassisBase
    {
       public:
        explicit Mecanum(const MecConfig &config);
        ~Mecanum() = default;
        [[noreturn]] void task() override;
        void init(const std::shared_ptr<Robot::Robot_set>& robot) override;

       private:
        void update_data();
        void decomposition_speed();

       private:
        fp32 vx_set_ = 0.f;
        // 前为正，单位 m/s
        fp32 vy_set_ = 0.f;
        // 左为正，单位 m/s
        fp32 wz_set_ = 0.f;
        fp32 wheel_speed_[4] = {};

        fp32 max_wheel_speed_ = 2.5f;
        Pid::Pid_rad chassis_angle_pid_;

        std::vector<Hardware::Motor> motors_;
    };
}  // namespace Chassis
