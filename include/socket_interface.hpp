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
    class Server_socket_interface
    {
       public:
        Server_socket_interface(std::shared_ptr<Robot::Robot_set> robot_set);
        ~Server_socket_interface();
        void task();
		inline void unpack();
		void lisent_connection();

       private:
        int64_t port_num;
        int server_socket_fd, client_socket_fd;
        sockaddr_in serv_addr, cli_addr;
        std::shared_ptr<Robot::Robot_set> p_robot_set;
        char buffer[256];

       public:
       private:
    };
}  // namespace Io

#endif
