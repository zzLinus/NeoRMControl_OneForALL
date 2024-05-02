#include "chassis_steering.hpp"
#include <cmath>
#include <type_traits>
#include <vector>
#include "config.hpp"
#include "hardware.hpp"
#include "motor.hpp"
#include "pid_controller.hpp"

namespace Chassis
{
    Chassis_steering::Chassis_steering() {
        // 电机初始int 
        speed_motors.assign(4, Hardware::Motor{ Config::M3508_SPEED_PID_CONFIG });
        turn_motors.assign(4, Hardware::Motor{ Config::M6020_ANGLE_PID_CONFIG });  //TODO:Config::M6020_ANGLE_PID_CONFIG
        turn_absolute_pid.assign(4, Pid::Pid_rad{ Config::TURN_ABSOLUTE_PID_CONFIG });
        turn_motor_forward_ecd = {
            Config::TURN_MOTOR_INIT_ECD_1,
            Config::TURN_MOTOR_INIT_ECD_2,
            Config::TURN_MOTOR_INIT_ECD_3,
            Config::TURN_MOTOR_INIT_ECD_4
        };
        inverse_flag.assign(4,0);
    }

    void Chassis_steering::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        for (size_t i = 0; i < speed_motors.size(); i++) { 
            auto &mot = speed_motors[i];
            Robot::hardware->register_callback<CAN0>(0x201 + i, [&mot](const auto &frame) { mot.unpack(frame); });
        }
        for (size_t j = 0; j < turn_motors.size(); j++) {
            auto &turn_mot = turn_motors[j];
            Robot::hardware->register_callback<CAN0>(0x205 + j, [&turn_mot](const auto &frame) { turn_mot.unpack(frame); });
        }
    }

    void Chassis_steering::control_loop() {
        decomposition_speed();
        update_data();
        if (no_force) {
            for (auto &m : speed_motors) {
                m.give_current = 0;
            }
            for (auto &turn_m : turn_motors) {
                turn_m.give_current = 0;
            }
        } else {
            fp32 max_speed = 0.f;
            for (int i = 0; i < 4; i++) {
                if (inverse_flag[i]) {
                    wheel_speed[i] = -wheel_speed[i];
                    if (wheel_angle[i] > 0) {
                        wheel_angle[i] = wheel_angle[i] - M_PIf;
                    } else {
                        wheel_angle[i] = wheel_angle[i] + M_PIf;
                    }
                }
            }
            for (int i = 0; i < 4; i++) {
                speed_motors[i].speed_set = wheel_speed[i];
                max_speed = std::max(max_speed, fabsf(wheel_speed[i]));
            }
            //let turn_motor rotate along the minor arc
            for (int i = 0; i < 4; i++) {
                turn_motors[i].angle_set = wheel_angle[i];
                if (turn_motors[i].angle_set - turn_motors[i].angle > M_PIf) { 
                    turn_motors[i].angle_set = turn_motors[i].angle_set - 2*M_PIf;
                } 
                else if (turn_motors[i].angle_set - turn_motors[i].angle < -M_PIf){
                    turn_motors[i].angle_set = turn_motors[i].angle_set + 2*M_PIf;
                }
            }

            if (max_speed > max_wheel_speed) {
                fp32 speed_rate = max_wheel_speed / max_speed;
                for (auto &m : speed_motors) {
                    m.speed_set *= speed_rate;
                }
            }
            //let M3508 inverse rotate when turn_angle bigger than 3*PI/4
            for (int i = 0; i < 4; i++) {
                if (turn_motors[i].angle_set > 0.75*M_PIf) {
                    turn_motors[i].angle_set = turn_motors[i].angle_set - M_PIf;
                    speed_motors[i].speed_set = -speed_motors[i].speed_set;           
                }
                else if (turn_motors[i].angle_set < -0.75*M_PIf) {
                    turn_motors[i].angle_set = turn_motors[i].angle_set + M_PIf;
                    speed_motors[i].speed_set = -speed_motors[i].speed_set;
                }
            }
    
            for (auto &m : speed_motors) {
                m.pid_ctrler.calc(m.speed, m.speed_set);
                m.give_current = (int16_t)(m.pid_ctrler.out);
            }
            for (int j = 0; j < 4; j++) {
                turn_absolute_pid[j].calc(turn_motors[j].angle, turn_motors[j].angle_set);
                turn_motors[j].speed_set = turn_absolute_pid[j].out;
                turn_motors[j].pid_ctrler.calc(turn_motors[j].angular_velocity, turn_motors[j].speed_set);
                turn_motors[j].give_current = (int16_t)turn_motors[j].pid_ctrler.out;
            }

        }
        Robot::hardware->send<CAN0>(Hardware::get_frame(0x200, speed_motors));
        Robot::hardware->send<CAN0>(Hardware::get_frame(0x2FF, turn_motors));
    }

    void Chassis_steering::decomposition_speed() {
        // The wheel order is the same as the quadrant order
        fp32 vector1x = vx_set - wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;   
        //The velocity component of the steering wheel 1 in the forward direction
        fp32 vector1y = vy_set + wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;    
        //The velocity component of the steering wheel 1 in the traversing direction
        fp32 vector2x = vx_set - wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;
        fp32 vector2y = vy_set - wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;
        fp32 vector3x = vx_set + wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;
        fp32 vector3y = vy_set - wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;
        fp32 vector4x = vx_set + wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;
        fp32 vector4y = vy_set + wz_set * Config::CHASSIS_RADIUS * std::sqrt(2) / 2;
        wheel_speed[0] = std::sqrt(vector1x * vector1x + vector1y * vector1y);
        wheel_speed[1] = std::sqrt(vector2x * vector2x + vector2y * vector2y);
        wheel_speed[2] = std::sqrt(vector3x * vector3x + vector3y * vector3y);
        wheel_speed[3] = std::sqrt(vector4x * vector4x + vector4y * vector4y);
        wheel_angle[0] = atan(vector1y / vector1x);
        wheel_angle[1] = atan(vector2y / vector2x);
        wheel_angle[2] = atan(vector3y / vector3x);
        wheel_angle[3] = atan(vector4y / vector4x);
    }


    //TODO
    void Chassis_steering::update_data() {
        for (auto &m : speed_motors) {
            m.speed = Config::CHASSIS_MOTOR_RPM_TO_VECTOR_SEN * (fp32)m.motor_measure.speed_rpm;
            m.accel = Config::CHASSIS_CONTROL_FREQUENCE * m.pid_ctrler.Dbuf;
        }
        for (int i = 0; i < 4; i++) {
            turn_motors[i].angle = Config::M6020_ECD_TO_RAD * ( turn_motors[i].motor_measure.ecd - turn_motor_forward_ecd[i] );
            turn_motors[i].angular_velocity = Config::RPM_TO_RAD_S * turn_motors[i].motor_measure.speed_rpm;
            if (speed_motors[i].motor_measure.speed_rpm < 0) {
                inverse_flag[i] = 1;
            } else {
                inverse_flag[i] = 0;
            }
        }
    }
}  // namespace Chassis
