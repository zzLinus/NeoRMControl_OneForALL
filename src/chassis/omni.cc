#include "chassis/mecanum.hpp"
#include "hardware.hpp"

namespace Chassis
{
    Mecanum::Mecanum(const MecConfig &config) : chassis_angle_pid_(config.follow_gimbal_pid_config) {
        motors_.assign(4, Hardware::Motor{ config.speed_pid_config });
    }

    void Mecanum::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set_ = robot;
        for (size_t i = 0; i < motors_.size(); i++) {
            auto &mot = motors_[i];
            Robot::hardware->register_callback<CAN1>(0x201 + i, [&mot](const auto &frame) { mot.unpack(frame); });
        }
    }

    [[noreturn]] void Mecanum::task() {
        while (true) {
            update_data();
            decomposition_speed();
            if (robot_set_->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                for (auto &m : motors_) {
                    m.give_current = 0;
                }
            } else {
                fp32 max_speed = *std::max_element(wheel_speed_, wheel_speed_ + 4);
                fp32 speed_rate = std::min(max_wheel_speed_ / max_speed, 1.f);
                for (auto &m : motors_) {
                    m.speed_set *= speed_rate;
                    m.pid_ctrler.calc(m.speed, m.speed_set);
                    m.give_current = (int16_t)(m.pid_ctrler.out);
                }
            }
            Robot::hardware->send<CAN1>(Hardware::get_frame(0x200, motors_));
            UserLib::sleep_ms(Config::CHASSIS_CONTROL_TIME);
        }
    }

    void Mecanum::decomposition_speed() {
        if (robot_set_->mode != Types::ROBOT_MODE::ROBOT_NO_FORCE) {
            fp32 sin_yaw, cos_yaw;
            sincosf(robot_set_->yaw_relative, &sin_yaw, &cos_yaw);
            vx_set_ = cos_yaw * robot_set_->vx_set - sin_yaw * robot_set_->vy_set;
            vy_set_ = sin_yaw * robot_set_->vx_set + cos_yaw * robot_set_->vy_set;

            if (robot_set_->mode == Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL) {
                chassis_angle_pid_.calc(robot_set_->yaw_relative, 0.f);
                wz_set_ = chassis_angle_pid_.out;
            } else {
                wz_set_ = robot_set_->wz_set;
            }
        }
        //TODO:try a new speed decomposition to decrease chassis power
        wheel_speed_[0] = -vx_set_ + vy_set_ + wz_set_;
        wheel_speed_[1] = vx_set_ + vy_set_ + wz_set_;
        wheel_speed_[2] = vx_set_ - vy_set_ + wz_set_;
        wheel_speed_[3] = -vx_set_ - vy_set_ + wz_set_;
    }

    void Mecanum::update_data() {
        for (auto &m : motors_) {
            m.speed = Config::CHASSIS_MOTOR_RPM_TO_VECTOR_SEN * (fp32)m.motor_measure.speed_rpm;
            m.accel = Config::CHASSIS_CONTROL_FREQUENCE * m.pid_ctrler.Dbuf;
        }
    }
}  // namespace Chassis
