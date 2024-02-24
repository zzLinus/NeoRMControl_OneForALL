#ifndef __IO__
#define __IO__

#include "kb_ctrl.hpp"
#include "types.hpp"
#include "ui.hpp"

namespace Io
{
    class Io_handler
    {
       public:
        Io_handler();
        ~Io_handler();
        void task(void);

       private:
       public:
        Input::Kb_ctrl* kb;
        Ui::Ncurses_ui* ui;

       private:
    };

}  // namespace Io

#endif
