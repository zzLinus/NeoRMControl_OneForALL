#ifndef __UI__
#define __UI__

#include <chrono>

#include "imtui/imtui-impl-ncurses.h"
#include "imtui/imtui.h"
#include "kb_ctrl.hpp"
#include "types.hpp"
#include <functional>

namespace Ui
{
    class Ncurses_ui
    {
       public:
        Ncurses_ui() = delete;
        Ncurses_ui(std::function<fp32(Types::Kb_event, fp32)>);
        ~Ncurses_ui();
        void render(void);

       private:
		fp32 cur_speed_x;
		fp32 cur_speed_y;
		std::function<fp32(Types::Kb_event, fp32)> input_handler;

       public:
        Types::debug_info_t* debug;

       private:
    };

}  // namespace Ui

#endif
