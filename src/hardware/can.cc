#include "can.hpp"

#include <cstring>

#include "iostream"

namespace Hardware
{
    Can_interface::Can_interface()
    {
        addr = new sockaddr_can;
        frame = new can_frame;
        frame_r = new can_frame;
        ifr = new ifreq;
        soket_id = -1;
        init_flag = false;
    }

    void Can_interface::init()
    {
        // create CAN socket
        if ((soket_id = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
        {
            perror("Error while creating socket");
            exit(-1);
        }

        struct can_filter rfilter[1];
        rfilter[0].can_id = 0x201;
        rfilter[0].can_mask = 0x3ff;
        setsockopt(soket_id, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

        std::strcpy(ifr->ifr_name, Config::CAN_CHANNEL);
        ioctl(soket_id, SIOCGIFINDEX, ifr);

        addr->can_family = AF_CAN;
        addr->can_ifindex = ifr->ifr_ifindex;

        // bind CAN socket to the given interface
        if (bind(soket_id, (sockaddr *)addr, sizeof(*addr)) < 0)
        {
            perror("Error in socket bind");
            exit(-1);
        }
        init_flag = true;
    }

    Can_interface::~Can_interface()
    {
        delete addr;
        delete frame;
        delete frame_r;
        delete ifr;
    }

    bool Can_interface::can_dump(Types::debug_info_t *debug)
    {
        for (;;)
        {
            if (init_flag != false)
            {
                debug->err = false;
                // read CAN frame
                if (read(soket_id, frame_r, sizeof(can_frame)) <= 0)
                {
                    perror("Error reading CAN frame");
                    return Status::ERROR;
                }
                else
                {
                    // printf("not reading!\n");
                }

                debug->can_f.can_id = frame_r->can_id;
                debug->can_f.can_dlc = frame_r->can_dlc;

                for (int i = 0; i < frame_r->can_dlc; i++)
                {
                    debug->can_f.data[i] = frame_r->data[i];
                }
            }
            debug->err = true;
        }
    }

    bool Can_interface::can_send(uint64_t pkg)
    {
        can_pkg = pkg;

        // eg : test can pkd <01BB117001BBEE90>
        frame->can_id = 0x200;
        frame->can_dlc = 8;
        frame->data[1] = (uint8_t)(can_pkg >> 0);
        frame->data[0] = (uint8_t)(can_pkg >> 8);
        frame->data[3] = (uint8_t)(can_pkg >> 16);
        frame->data[2] = (uint8_t)(can_pkg >> 24);
        frame->data[5] = (uint8_t)(can_pkg >> 32);
        frame->data[4] = (uint8_t)(can_pkg >> 40);
        frame->data[7] = (uint8_t)(can_pkg >> 48);
        frame->data[6] = (uint8_t)(can_pkg >> 56);

        /* send CAN frame */
        write(soket_id, frame, sizeof(can_frame));

        return true;
    }

}  // namespace Hardware
