#include "shoot.hpp"

namespace Shoot
{

    Shoot::Shoot() : friction_ramp(Config::FRICTION_ADD_SPEED, Config::SHOOT_CONTROL_TIME * 1e-3f) {
        friction.assign(Config::FRICTION_NUM, Hardware::Motor{ Config::FRIC_SPEED_PID_CONFIG });
        trigger.assign(Config::TRIGGER_NUM, Hardware::Motor{ Config::TRIGGER_SPEED_PID_CONFIG });
    }

    void Shoot::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        friction_sender.init_can(0x200,"can1");
        trigger_sender.init_can(0x1ff,"can1");

        for (int i = 0; i < friction.size(); i++) {
            auto &mot = friction[i];
            friction_sender.addMotor(mot,0x201 + i);
        }
        for (int i = 0; i < trigger.size(); i++) {
            auto &mot = trigger[i];
            trigger_sender.addMotor(mot,0x205 + i);
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
        fp32 trigger_speed = 0.f;
        if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
            friction_ramp.clear();
            trigger_speed = 0.f;
        } else {
            friction_ramp.update(robot_set->friction_open ? Config::FRICTION_MAX_SPEED : 0.f);
            if(back_time) {
                back_time--;
                trigger_speed = robot_set->shoot_open ? -Config::CONTINUE_TRIGGER_SPEED : 0.f;
            }
            else {
                if(isJam()) {
                    jam_time++;
                }
                if(jam_time > 500) {
                    back_time = 1000;
                    jam_time = 0;
                }
                trigger_speed = trigger[1].speed_set = robot_set->shoot_open ? Config::CONTINUE_TRIGGER_SPEED : 0.f;
            }
        }
        friction[0].speed_set = -friction_ramp.out;
        friction[1].speed_set = friction_ramp.out;
        friction[2].speed_set = friction_ramp.out;
        friction[3].speed_set = -friction_ramp.out;
        for(auto & mot : trigger) {
            mot.speed_set = trigger_speed;
        }
    }

    [[noreturn]] void Shoot::task() {
        while(true) {
            update_speed();
            decomposition_speed();
            if(robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                for(auto & mot : friction) {
                    mot.give_current = 0;
                }
            } else {
                for (auto &mot : friction) {
                    mot.pid_ctrler.calc(mot.speed, mot.speed_set);
                    mot.give_current = (int16_t)mot.pid_ctrler.out;
                }
            }
            if(robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE || !robot_set->shoot_open) {
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
            friction_sender.send();
            trigger_sender.send();
            UserLib::sleep_ms(Config::SHOOT_CONTROL_TIME);
        }
    }

    bool Shoot::isJam() {
        return (trigger[0].give_current > 4000) && (trigger[1].give_current > 4000) &&
               (trigger[0].speed < 1.f) && (trigger[1].speed < 1.f);
    }
}  // namespace Shoot
