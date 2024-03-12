#ifndef __CAN_INTERFACE__
#define __CAN_INTERFACE__

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <functional>

#include "config.hpp"
#include "string"
#include "utils.hpp"
#include "hardware_callback.hpp"

namespace Hardware
{
    class Can_interface : public Callback_key<uint32_t , can_frame>
    {
       public:
        Can_interface();
        ~Can_interface();
        bool send(const can_frame &frame);
        bool task();
        void init(const char *can_channel);

       private:
        sockaddr_can *addr;
        can_frame frame_r;
        ifreq *ifr;
        Types::debug_info_t *debug;
        int soket_id;
        bool init_flag;

       public:
    };

}  // namespace Hardware

#endif
