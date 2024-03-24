#include <iostream>

#include "iostream"
#include "robot_controller.hpp"
#include "thread"

using namespace std::chrono;

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;

    robot.load_hardware();
    robot.start_init();
    robot.init_join();
    LOG_INFO("finish init\n");

	 robot.robot_set->mode = Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL;
	 robot.robot_set->yaw_set = robot.robot_set->ins_roll;
	 robot.robot_set->friction_open = true;

	 robot.start();
	 robot.join();

    return 0;
}
