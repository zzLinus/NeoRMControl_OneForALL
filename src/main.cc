#include <iostream>

#include "io.hpp"
#include "iostream"
#include "robot_controller.hpp"
#include "thread"

using namespace std::chrono;

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;

    robot.load_hardware();
    robot.start();
    // robot.start_init();

    robot.join();
    // robot.init_join();

    return 0;
}
