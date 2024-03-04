#ifndef __IO__
#define __IO__

#include <memory>

#include "kb_ctrl.hpp"
#include "robot.hpp"
#include "socket_interface.hpp"
#include "types.hpp"
#include "ui.hpp"

namespace Io
{
    class Io_handler
    {
       public:
        Io_handler(std::shared_ptr<Robot::Robot_set> robot_set);
        ~Io_handler();
        void task();

       private:
       public:
        Input::Kb_ctrl* kb;
        Ui::raylib_ui* ui;

       private:
    };

}  // namespace Io

#endif
