#include "io.hpp"

#include <chrono>
#include <thread>

namespace Io
{
    Io_handler::Io_handler(std::shared_ptr<Robot::Robot_set> robot_set) {
        p_robot_set = robot_set;
        kb = new Input::Kb_ctrl(p_robot_set);
        ui = new Ui::Ncurses_ui([&](Types::Kb_event event, fp32 spdslider) {
            return kb->event_handler(event, spdslider);
        });
    }

    Io_handler::~Io_handler() {
        delete ui;
        delete kb;
    }

    void Io_handler::task() {
        ui->init();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        while (true) {
            if (!ui->render())
                break;
        }
    }
}  // namespace Io
