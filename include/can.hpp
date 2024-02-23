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

#include "stdlib.h"
#include "string"
#include "types.hpp"

namespace Hardware
{
    class Can_interface
    {
       public:
        Can_interface();
        ~Can_interface();
        void init();
        bool can_send(uint64_t pkg);
        bool can_dump(Types::debug_info_t *debug);

       private:
        sockaddr_can *addr;
        can_frame *frame;
        ifreq *ifr;
		Types::debug_info_t *debug;
        int soket_id;
		uint64_t can_pkg;

       public:
       private:
    };

}  // namespace Hardware

#endif
