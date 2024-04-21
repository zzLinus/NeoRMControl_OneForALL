#pragma once

#include "can.hpp"
#include "serial_interface.hpp"
#include "socket_interface.hpp"
#include "thread"

using CAN = Hardware::Can_interface;
using SER = Hardware::Serial_interface<Types::ReceivePacket>;
using SOCKET = Io::Server_socket_interface;

template<typename T>
class IO_manager : public std::vector<T*>{
   public:
    using std::vector<T*>::vector;
    std::vector<std::thread> thread_list;

    void start() {
        for(auto & x : *this) {
            thread_list.emplace_back(&T::task, x);
        }
    }

    void join() {
        for(auto & x : thread_list) {
            x.join();
        }
    }
};

namespace Robot
{
    template<typename T>
    IO_manager<T> io;
}  // namespace Robot
