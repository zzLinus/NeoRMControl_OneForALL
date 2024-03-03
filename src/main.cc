#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "serial_interface.hpp"
#include "thread"
#include "types.hpp"

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;
    Hardware::Serial_interface<Types::ReceivePacket> serial;

    Io::Io_handler io(robot.robot_set);

    robot.start();

    std::thread io_thread(&Io::Io_handler::task, &io);
    std::thread serial_thread(&Hardware::Serial_interface<Types::ReceivePacket>::task, &serial);

    io_thread.join();
    return 0;
}
