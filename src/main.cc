#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "thread"

int main() {
    Robot::Robot_ctrl robot;
    Io::Io_handler io(robot.robot_set);

    robot.start();

    std::thread io_thread(&Io::Io_handler::task, &io);

    io_thread.join();
    return 0;
}
