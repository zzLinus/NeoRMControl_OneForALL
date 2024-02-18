#include "chassis_task.hpp"

#include "iostream"

namespace Chassis
{
    Chassis_task::Chassis_task()
    {
        cc = new Chassis_ctrl();
    }

    Chassis_task::~Chassis_task()
    {
        delete cc;
    }

    void Chassis_task::init()
    {
        // chassis init
        // 底盘初始化
        cc->init();

        // update data
        // 更新一下数据
        cc->control_get_feedback();
    }

    void Chassis_task::task(void)
    {
        init();
        // TODO: wait for initialization
        // vTaskDelay(CHASSIS_TASK_INIT_TIME);
        // TODO:make sure all chassis motor is online,
        // 判断底盘电机是否都在线
        while (!cc->is_motor_online())
        {
            // TODO: spin or error handling should be done here
            // vTaskDelay(CHASSIS_CONTROL_TIME_MS);
        }

        while (1)
        {
            // TODO: set chassis control mode
            // 设置底盘控制模式
            cc->set_mode();

            // 底盘数据更新
            cc->control_get_feedback();
            // chassis_feedback_update(&chassis_move);
            //  set chassis control set-point
            //  底盘控制量设置
            cc->control_set_target();
            // chassis_set_contorl(&chassis_move);
            //  chassis control pid calculate
            //  底盘控制PID计算
            cc->control_calc_pid();
            // chassis_control_loop(&chassis_move);

            // make sure  one motor is online at least, so that the control CAN message can be received
            // 确保至少一个电机在线， 这样CAN控制包可以被接收到
            if (cc->is_motor_online())
            {
                // TODO: move this else where
                // 当遥控器掉线的时候，发送给底盘电机零电流.
                // if (toe_is_error(DBUS_TOE))
                //{
                //    CAN_cmd_chassis(0, 0, 0, 0);
                //}
                // else
                //{
                // send control current
                // 发送控制电流
                cc->set_motor_current();
            }

            // std::cout << "Chassis task\n";
        }
    }

}  // namespace Chassis
