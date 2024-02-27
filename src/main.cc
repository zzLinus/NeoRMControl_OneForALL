#include <iostream>
#include <thread>

#include "chassis_task.hpp"
#include "gimbal_task.hpp"
#include "io.hpp"

int main(int argv, char *argc[])
{
    Chassis::Chassis_task chassis;
    Gimbal::Gimbal_info gimbal;
    Io::Io_handler io;

    std::thread io_t(&Io::Io_handler::task, io);
    std::thread gimbal_t(&Gimbal::Gimbal_info::task, gimbal);
    std::thread chassis_t(&Chassis::Chassis_task::task, chassis, io.kb->rc_ctrl, io.ui->debug);

    std::thread candump_t(
        &Hardware::Can_interface::can_dump, chassis.cc->can_itrf, io.ui->debug);  // keyboard input thread

    chassis_t.join();
    gimbal_t.join();
    candump_t.join();
    io_t.join();  // NOTE: render ui && handle keyboart event
}
