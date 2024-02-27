#ifndef __UI__
#define __UI__

#include <chrono>
#include <functional>

#include "imtui/imtui-impl-ncurses.h"
#include "imtui/imtui.h"
#include "kb_ctrl.hpp"
#include "types.hpp"

namespace Ui
{
    class Ncurses_ui
    {
       public:
        using InputHandler = std::function<fp32(Types::Kb_event, fp32)>;

        Ncurses_ui() = delete;
        Ncurses_ui(const InputHandler& event_handler);
        ~Ncurses_ui();
        void render();

       private:
        fp32 cur_speed_x;
        fp32 cur_speed_y;
        InputHandler input_handler;

       public:
        Types::debug_info_t* debug;

       private:
    };

}  // namespace Ui

#endif
