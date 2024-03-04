#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "serial_interface.hpp"
#include "thread"
#include "types.hpp"

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;
    Io::Socket_interface socket_intrf;

    Io::Io_handler io(robot.robot_set);

    robot.start();

    std::thread io_thread(&Io::Io_handler::task, &io);
    std::thread socket_thread(&Io::Socket_interface::task, &socket_intrf);

    try {
        Hardware::Serial_interface<Types::ReceivePacket> serial;
        std::thread serial_thread(&Hardware::Serial_interface<Types::ReceivePacket>::task, &serial);
    } catch (serial::IOException) {
        printf("there's no such serial device\n");
    }

    io_thread.join();
    return 0;
}
