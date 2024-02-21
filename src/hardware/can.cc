#include "can.hpp"

#include <cstring>

#include "iostream"

namespace Hardware
{
    Can_interface::Can_interface()
    {
        addr = new sockaddr_can;
        frame = new can_frame;
        ifr = new ifreq;
        soket_id = -1;
    }

    void Can_interface::init()
    {
        // create CAN socket
        if ((soket_id = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
        {
            perror("Error while creating socket");
            exit(-1);
        }

        // get interface index
        std::strcpy(ifr->ifr_name, Config::CAN_CHANNEL);
        ioctl(soket_id, SIOCGIFINDEX, ifr);

        addr->can_family = AF_CAN;
        addr->can_ifindex = ifr->ifr_ifindex;

        // bind CAN socket to the given interface
        if (bind(soket_id, (sockaddr*)addr, sizeof(*addr)) < 0)
        {
            perror("Error in socket bind");
            exit(-1);
        }
    }

    Can_interface::~Can_interface()
    {
        delete addr;
        delete frame;
        delete ifr;
    }

    bool Can_interface::can_send()
    {
        int nbytes = -1;
        int ret = Status::ERROR;

        // eg : test can pkd <01BB117001BBEE90>
        frame->can_id = 0x200;
        frame->can_dlc = 8;
        frame->data[0] = 0x01;
        frame->data[1] = 0xBB;
        frame->data[2] = 0x11;
        frame->data[3] = 0x70;
        frame->data[4] = 0x01;
        frame->data[5] = 0xBB;
        frame->data[6] = 0xEE;
        frame->data[7] = 0x90;

        /* send CAN frame */
        nbytes = write(soket_id, frame, sizeof(can_frame));

        if (nbytes != -1)
        {
#ifdef DEBUG
            std::cout << "Wrote " << nbytes << " bytes\n";
#endif
            ret = Status::OK;
        }

        return ret;
    }

}  // namespace Hardware
