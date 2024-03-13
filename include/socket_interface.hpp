#ifndef __SOCKET_INTERFACE__
#define __SOCKET_INTERFACE__

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <memory>

#include "robot.hpp"
#include "utils.hpp"

namespace Io
{
    class Server_socket_interface : public Hardware::Callback<Robot::Vison_control>

    {
       public:
        Server_socket_interface(std::shared_ptr<Robot::Robot_set> robot_set);
        ~Server_socket_interface();
        void task();

       private:
        int64_t port_num;
        int sockfd;
        socklen_t cli_addr_len;
        sockaddr_in serv_addr, cli_addr;
        std::shared_ptr<Robot::Robot_set> p_robot_set;
        char buffer[256];

       public:
       private:
    };
}  // namespace Io

#endif
