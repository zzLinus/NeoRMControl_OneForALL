#include "io.hpp"

namespace Io
{
    Io_handler::Io_handler()
    {
        kb = new Input::Kb_ctrl();
        ui = new Ui::Ncurses_ui(kb->event_handler);
    }

    Io_handler::~Io_handler()
    {
        delete kb;
        delete ui;
    }

    void Io_handler::task(void)
    {
        while (true)
        {
            ui->render();
        }
    }
}  // namespace Io
