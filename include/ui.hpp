#ifndef __UI__
#define __UI__

#include <bits/chrono.h>

#include "imtui/imtui-impl-ncurses.h"
#include "imtui/imtui.h"
#include "kb_ctrl.hpp"
#include "types.hpp"

namespace Ui
{
    class Ncurses_ui
    {
       public:
        Ncurses_ui() = delete;
        Ncurses_ui(fp32 (*event_handler)(Types::Kb_event event, fp32 spdslider));
        ~Ncurses_ui();
        void render(void);

       private:
        fp32 (*input_handler)(Types::Kb_event event, fp32 spdslider);
		fp32 cur_speed_x;
		fp32 cur_speed_y;

       public:
        Types::debug_info_t* debug;

       private:
    };

}  // namespace Ui

#endif
