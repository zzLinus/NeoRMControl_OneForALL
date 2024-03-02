#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "serial_interface.hpp"
#include "thread"

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;
    Hardware::Serial_interface serial;

    Io::Io_handler io(robot.robot_set);

    robot.start();

    std::thread io_thread(&Io::Io_handler::task, &io);
    std::thread serial_thread(&Hardware::Serial_interface::task, &serial);

    io_thread.join();
    return 0;
}
