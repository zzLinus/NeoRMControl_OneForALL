#ifndef __KB_CTRL__
#define __KB_CTRL__

#include "imtui/imtui-impl-ncurses.h"
#include "imtui/imtui.h"
#include "types.hpp"

namespace Controller
{
    class Kb_ctrl
    {
       public:
        Kb_ctrl();
        ~Kb_ctrl();
        void task();

       private:
       public:
        Types::RC_ctrl_t *rc_ctrl;
        Types::debug_info_t *debug;

       private:
    };
}  // namespace Controller

#endif
