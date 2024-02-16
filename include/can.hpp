#ifndef __CAN_INTERFACE__
#define __CAN_INTERFACE__

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "types.hpp"
#include "string"
#include "stdlib.h"

namespace Hardware
{
    class Can_interface
    {
       public:
        Can_interface();
        ~Can_interface();
		void init();
		bool can_send();

       private:
        sockaddr_can *addr;
        can_frame *frame;
        ifreq *ifr;
        int soket_id;

       public:
       private:
    };

}  // namespace Hardware

#endif
