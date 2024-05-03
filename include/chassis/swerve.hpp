#pragma once
#include "chassis_config.hpp"
#include "robot.hpp"
#include "motor.hpp"

#include <memory>

namespace Chassis
{
    class Swerve : public ChassisBase
    {
       public:
        explicit Swerve(const SwerveConfig &config);
        ~Swerve() = default;
        void init(const std::shared_ptr<Robot::Robot_set> &robot) override;
        [[noreturn]] void task() override;

       private:
        void update_data();
        void decomposition_speed();
        fp32 get_turn_value(fp32 angle_change, fp32 speed_change);
        void set_speed(uint32_t idx, const Vec2f &speed);

       private:
        fp32 vx_set_ = 0.f;
        fp32 vy_set_ = 0.f;
        fp32 wz_set_ = 0.f;

        Pid::Pid_rad chassis_angle_pid;

        std::vector<Hardware::Motor> speed_motors_;
        std::vector<Hardware::Motor> turn_motors_;

        std::vector<Pid::Pid_rad> turn_pid_;
        std::array<fp32, 4> turn_angle_;
        std::array<uint16_t, 4> turn_init_ecd_;
        std::array<fp32, 4> turn_angle_set_;

        std::array<Vec2f, 4> turn_norm_vector_ {
            Vec2f{M_SQRT1_2f, M_SQRT1_2f},
            Vec2f{M_SQRT1_2f, -M_SQRT1_2f},
            Vec2f{-M_SQRT1_2f, -M_SQRT1_2f},
            Vec2f{-M_SQRT1_2f, M_SQRT1_2f},
        };
    };
}  // namespace Chassis