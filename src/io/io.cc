#include "io.hpp"

#include <chrono>
#include <thread>

namespace Io
{
    Io_handler::Io_handler(std::shared_ptr<Robot::Robot_set> robot_set) {
        kb = new Input::Kb_ctrl();

        robot_set->mode = Types::ROBOT_NOT_FOLLOW;
    }

    Io_handler::~Io_handler() {
        delete kb;
    }

    void Io_handler::task() {
        //ui->init();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        while (true) {
            //if (!ui->render())
            //    break;
        }
    }
}  // namespace Io
