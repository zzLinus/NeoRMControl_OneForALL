#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "serial_interface.hpp"
#include "thread"
#include "types.hpp"

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;
    Io::Server_socket_interface socket_intrf(robot.robot_set);

    Io::Io_handler io(robot.robot_set);

    robot.start();

    std::thread io_thread(&Io::Io_handler::task, &io);
    std::thread socket_thread(&Io::Server_socket_interface::task, &socket_intrf);

    try {
        Hardware::Serial_interface<Types::ReceivePacket> serial;
        std::thread serial_thread(&Hardware::Serial_interface<Types::ReceivePacket>::task, &serial);
        serial_thread.join();
    } catch (serial::IOException) {
        printf("there's no such serial device\n");
    }

    io_thread.join();
    return 0;
}
