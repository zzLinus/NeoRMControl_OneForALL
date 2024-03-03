#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "serial_interface.hpp"
#include "thread"
#include "types.hpp"
#include "utils.hpp"

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;
    Io::Server_socket_interface socket_intrf(robot.robot_set);

    Io::Io_handler io(robot.robot_set);

    auto copySerialData = [&]() {

        fp32 angle = 0.f, da = 0.001f;

        while(true) {
            robot.robot_set->ins_yaw = serial.rp.yaw;
            robot.robot_set->ins_pitch = serial.rp.pitch;
            robot.robot_set->ins_roll = serial.rp.roll;
            robot.robot_set->mode = Types::ROBOT_MODE::ROBOT_NOT_FOLLOW;

//            if(fabs(angle) >= 2.f) {
//                da *= -1;
//            }
//            angle += da;
//            robot.robot_set->v_yaw_set = angle;

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    };

    robot.start();

    std::thread io_thread(&Io::Io_handler::task, &io);
    std::thread socket_thread(&Io::Server_socket_interface::task, &socket_intrf);

    try {
        Hardware::Serial_interface<Types::ReceivePacket> serial;
        std::thread serial_thread(&Hardware::Serial_interface<Types::ReceivePacket>::task, &serial);
        serial_thread.join();
    } catch (serial::IOException) {
        LOG_ERR("there's no such serial device\n");
    }

    io_thread.join();
    return 0;
}
