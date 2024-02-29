#ifndef __IO__
#define __IO__

#include <memory>

#include "kb_ctrl.hpp"
#include "robot.hpp"
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
        std::shared_ptr<Robot::Robot_set> p_robot_set;

       public:
        Input::Kb_ctrl* kb;
        Ui::Ncurses_ui* ui;

       private:
    };

}  // namespace Io

#endif
