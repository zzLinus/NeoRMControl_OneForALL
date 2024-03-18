#include "socket_interface.hpp"

#include <chrono>
#include <thread>

#include "user_lib.hpp"

namespace Io
{
    void Server_socket_interface::task() {
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            sockaddr_in cli_addr;
            socklen_t cli_addr_len = sizeof(cli_addr);
            auto n = recvfrom(sockfd, buffer, 256, MSG_WAITALL, (sockaddr *)&cli_addr, &cli_addr_len);
            if (n > 0) {
                uint8_t header = buffer[0];
                if (clients.count(header) == 0) {
                    LOG_OK("register clients %d\n", header);
                    clients.insert(std::pair<uint8_t, sockaddr_in>(header, cli_addr));
                }
                switch (header) {
                    case 0xA5: {
                        Robot::ReceiveGimbalPacket pkg{};
                        UserLib::unpack(pkg, buffer);
                        callback(pkg);
                        break;
                    }
                    case 0x6A: {
                        Robot::Vison_control vc;
                        UserLib::unpack(vc, buffer);
                        callback(vc);
                        break;
                    }
                    default: {
                        LOG_ERR("get error flag: %02x\n", header);
                        Robot::ReceiveGimbalPacket pkg{};
                        UserLib::unpack(pkg, buffer);
                        break;
                    }
                }
            }
        }
    }

    Server_socket_interface::Server_socket_interface() : port_num(51718) {
        // NOTE: read this https://www.linuxhowtos.org/C_C++/socket.htm
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            LOG_ERR("can't open socket\n");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port_num);

        connections.insert(std::pair<uint8_t, uint8_t>(0xA6, 0x6A));
        connections.insert(std::pair<uint8_t, uint8_t>(0x5A, 0xA5));

        if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            LOG_ERR("can't bind socket fd with port number");
        }
    }

    Server_socket_interface::~Server_socket_interface() {
        close(sockfd);
    }
}  // namespace Io
