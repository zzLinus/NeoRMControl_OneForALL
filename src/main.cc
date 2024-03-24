#include <iostream>

#include "iostream"
#include "robot_controller.hpp"
#include "thread"

using namespace std::chrono;

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;

    robot.load_hardware();
    robot.start_init();
//    robot.init_join();
//    LOG_INFO("finish init\n");
//
    robot.robot_set->mode = Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL;
//    robot.robot_set->yaw_set = robot.robot_set->ins_roll;
//
    robot.start();

    while (true) {
        uint32_t type;
        std::cin >> type;
        if(type == 1) {
            robot.robot_set->friction_open ^= 1;
        }
        if(type == 2) {
            robot.robot_set->shoot_open ^= 1;
        }
    }
//    robot.join();

    return 0;
}
