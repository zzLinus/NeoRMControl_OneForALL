#pragma once

#include "can.hpp"
#include "serial_interface.hpp"
#include "socket_interface.hpp"
#include "hardware_manager.hpp"

namespace Robot
{
    using RobotHardware = Hardware::Hardware_manager<
        Hardware::Can_interface,
        Hardware::Can_interface,
        Hardware::Serial_interface<Types::ReceivePacket>,
        Io::Server_socket_interface>;

    extern std::shared_ptr<RobotHardware> hardware;
}