#include "shoot.hpp"

namespace Shoot
{

    Shoot::Shoot() : friction_ramp(Config::FRICTION_ADD_SPEED, Config::SHOOT_CONTROL_TIME * 1e-3f) {
        friction.assign(Config::FRICTION_NUM, Hardware::Motor{ Config::FRIC_SPEED_PID_CONFIG });
        trigger.assign(Config::TRIGGER_NUM, Hardware::Motor{ Config::TRIGGER_SPEED_PID_CONFIG });
    }

    void Shoot::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        for (int i = 0; i < friction.size(); i++) {
            auto &mot = friction[i];
            Robot::hardware->register_callback<CAN0>(
                0x201 + i, [&mot](const auto &frame) { return mot.unpack(frame); });
        }
        for (int i = 0; i < trigger.size(); i++) {
            auto &mot = trigger[i];
            Robot::hardware->register_callback<CAN0>(
                0x205 + i, [&mot](const auto &frame) { return mot.unpack(frame); });
        }
    }

    void Shoot::update_speed() {
        for (auto &m : friction) {
            m.speed = m.motor_measure.speed_rpm;
        }
        for (auto &m : trigger) {
            m.speed = Config::SHOOT_MOTOR_RPM_TO_SPEED * (fp32)m.motor_measure.speed_rpm;
        }
    }

    void Shoot::decomposition_speed() {
        if (no_force) {
            friction_ramp.clear();
            trigger[0].speed_set = trigger[1].speed_set = 0.f;
        } else {
            friction_ramp.update(friction_open ? Config::FRICTION_MAX_SPEED : 0.f);
            trigger[0].speed_set = trigger[1].speed_set = shoot_open ? Config::CONTINUE_TRIGGER_SPEED : 0.f;
        }
        friction[0].speed_set = -friction_ramp.out;
        friction[1].speed_set = friction_ramp.out;
        friction[2].speed_set = friction_ramp.out;
        friction[3].speed_set = -friction_ramp.out;
    }

    void Shoot::control_loop() {
        update_speed();
        decomposition_speed();
        if (no_force) {
            for(auto & mot : friction) {
                mot.give_current = 0;
            }
        } else {
            for (auto &mot : friction) {
                mot.pid_ctrler.calc(mot.speed, mot.speed_set);
                mot.give_current = (int16_t)mot.pid_ctrler.out;
            }
        }
        if(no_force || !shoot_open) {
            for(auto & mot : trigger) {
                mot.give_current = 0;
            }
        }
        else {
            for (auto &mot : trigger) {
                mot.pid_ctrler.calc(mot.speed, mot.speed_set);
                mot.give_current = (int16_t)mot.pid_ctrler.out;
            }
        }
        Robot::hardware->send<CAN0>(Hardware::get_frame(0x200, friction));
        Robot::hardware->send<CAN0>(Hardware::get_frame(0x1FF, trigger));
    }
}  // namespace Shoot