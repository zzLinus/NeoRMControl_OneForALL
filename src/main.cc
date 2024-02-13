#include <iostream>
#include <thread>

#include "chassis_task.hpp"
#include "gimbal_task.hpp"

int main(int argv, char *argc[])
{
    Chassis::Chassis_task chassis;
    Gimbal::Gimbal_info gimbal;

    std::thread ct(&Chassis::Chassis_task::task, chassis);
    std::thread gt(&Gimbal::Gimbal_info::Gimbal_task, gimbal);

    ct.join();
    gt.join();
}
