#include "socket_interface.hpp"

namespace Io
{
    void Socket_interface::task() {
        listen(server_socket_fd, 5);
        socklen_t clilen = sizeof(cli_addr);
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&cli_addr, &clilen);
        if (client_socket_fd < 0) {
            printf("ERROR on accept message\n");
        }
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int n = read(client_socket_fd, buffer, 255);
            if (n < 0) {
                printf("ERROR reading from socket");
            }
            printf("Here is the message: %s\n", buffer);
            n = write(client_socket_fd, "I got your message", 18);
            if (n < 0)
                printf("ERROR writing to socket");
        }
    }

    Socket_interface::Socket_interface() : port_num(51717) {
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_fd < 0) {
            printf("can't open socket\n");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port_num);

        if (bind(server_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("can't bind socket fd with port number");
        }
    }

    Socket_interface::~Socket_interface() {
        close(client_socket_fd);
        close(server_socket_fd);
    }
}  // namespace Io
