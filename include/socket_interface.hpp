#ifndef __SOCKET_INTERFACE__
#define __SOCKET_INTERFACE__

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

namespace Io
{

    class Socket_interface
    {
       public:
        Socket_interface();
        ~Socket_interface();
        void task();

       private:
        int64_t port_num;
        int server_socket_fd, client_socket_fd;
        sockaddr_in serv_addr, cli_addr;
        char buffer[256];

       public:
       private:
    };
}  // namespace Io

#endif
