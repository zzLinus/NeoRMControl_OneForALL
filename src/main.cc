#include <iostream>
#include <thread>

#include "chassis_task.hpp"
#include "gimbal_task.hpp"
#include "kb_ctrl.hpp"

int main(int argv, char *argc[])
{
    Chassis::Chassis_task chassis;
    Gimbal::Gimbal_info gimbal;
    Controller::Kb_ctrl kb_ctrl;

    std::thread ct(&Chassis::Chassis_task::task, chassis);
    std::thread gt(&Gimbal::Gimbal_info::task, gimbal);
    std::thread kt(&Controller::Kb_ctrl::task, kb_ctrl);  // keyboard input thread

    ct.join();
    gt.join();
}
