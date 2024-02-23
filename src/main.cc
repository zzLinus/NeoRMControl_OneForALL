#include <iostream>
#include <thread>

#include "can.hpp"
#include "chassis_task.hpp"
#include "gimbal_task.hpp"
#include "kb_ctrl.hpp"

int main(int argv, char *argc[])
{
    Chassis::Chassis_task chassis;
    Gimbal::Gimbal_info gimbal;
    Input::Kb_ctrl kb_ctrl;

    std::thread chassis_t(&Chassis::Chassis_task::task, chassis, &kb_ctrl);
    std::thread gimbal_t(&Gimbal::Gimbal_info::task, gimbal);
    std::thread kb_t(&Input::Kb_ctrl::task, kb_ctrl);  // keyboard input thread
    std::thread candump_t(
        &Hardware::Can_interface::can_dump, chassis.cc->can_itrf, kb_ctrl.debug);  // keyboard input thread

    chassis_t.join();
    gimbal_t.join();
    kb_t.join();
    candump_t.join();
}
