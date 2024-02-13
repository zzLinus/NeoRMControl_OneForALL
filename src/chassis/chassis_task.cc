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

    void Chassis_task::update_feedback()
    {
    }

    void Chassis_task::init()
    {
        // chassis init
        // 底盘初始化
        cc->init();

        // in beginning， chassis mode is raw
        // 底盘开机状态为原始
        cc->chassis_mode = CHASSIS_VECTOR_RAW;
        // get remote control point
        // TODO: 获取遥控器指针
        // cc->chassis_RC = get_remote_control_point();
        // get gyro sensor euler angle point
        // TODO: 获取陀螺仪姿态角指针
        // cc->chassis_INS_angle = get_INS_angle_point();
        // get gimbal motor data point
        // TODO:获取云台电机数据指针
        // cc->chassis_yaw_motor = get_yaw_motor_point();
        // cc->chassis_pitch_motor = get_pitch_motor_point();

        // get chassis motor data point,  initialize motor speed PID
        // 获取底盘电机数据指针，初始化PID
        // WARN: PID应该被封装到电机里还是？
        for (auto& m : cc->mecanum_wheel)
        {
            m->pid_ctrl.init();
        }
        // for (i = 0; i < 4; i++)
        //{
        //  PID_init(
        //      &chassis_move_init->motor_speed_pid[i],
        //      PID_POSITION,
        //      motor_speed_pid,
        //      M3505_MOTOR_SPEED_PID_MAX_OUT,
        //      M3505_MOTOR_SPEED_PID_MAX_IOUT);
        //}

        // HACK:initialize angle PID
        // 初始化角度PID
        cc->follow_angle_pid.init();
        cc->no_follow_angle_pid.init();
        // PID_init(
        //     &chassis_move_init->chassis_angle_pid,
        //     PID_POSITION,
        //     chassis_yaw_pid,
        //     CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT,
        //     CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT);
        //  初始化chassis_no_follow_angle_pid
        // PID_init(
        //     &chassis_move_init->chassis_no_follow_angle_pid,
        //     PID_POSITION,
        //     chassis_no_follow_yaw_pid,
        //     CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT,
        //     CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT);

        // max and min speed
        // 最大 最小速度
        cc->vx_max_speed = NORMAL_MAX_CHASSIS_SPEED_X;
        cc->vx_min_speed = -NORMAL_MAX_CHASSIS_SPEED_X;

        cc->vy_max_speed = NORMAL_MAX_CHASSIS_SPEED_Y;
        cc->vy_min_speed = -NORMAL_MAX_CHASSIS_SPEED_Y;

        // update data
        // 更新一下数据
        update_feedback();
    }

    void Chassis_task::task(void)
    {
        // TODO: wait for initialization
        // vTaskDelay(CHASSIS_TASK_INIT_TIME);
        // make sure all chassis motor is online,
        // 判断底盘电机是否都在线
        // while (toe_is_error(CHASSIS_MOTOR1_TOE) || toe_is_error(CHASSIS_MOTOR2_TOE) ||
        //        toe_is_error(CHASSIS_MOTOR3_TOE) || toe_is_error(CHASSIS_MOTOR4_TOE) || toe_is_error(DBUS_TOE))
        //{
        //     vTaskDelay(CHASSIS_CONTROL_TIME_MS);
        // }
        while (1)
        {
            // set chassis control mode
            // 设置底盘控制模式
            // chassis_set_mode(&chassis_move);
            // when mode changes, some data save
            // 模式切换数据保存
            // chassis_mode_change_control_transit(&chassis_move);
            // chassis data update
            // 底盘数据更新
            // chassis_feedback_update(&chassis_move);
            // set chassis control set-point
            // 底盘控制量设置
            // chassis_set_contorl(&chassis_move);
            // chassis control pid calculate
            // 底盘控制PID计算
            // chassis_control_loop(&chassis_move);

            // make sure  one motor is online at least, so that the control CAN message can be received
            // 确保至少一个电机在线， 这样CAN控制包可以被接收到
            // if (!(toe_is_error(CHASSIS_MOTOR1_TOE) && toe_is_error(CHASSIS_MOTOR2_TOE) &&
            //      toe_is_error(CHASSIS_MOTOR3_TOE) && toe_is_error(CHASSIS_MOTOR4_TOE)))
            //{
            //    // when remote control is offline, chassis motor should receive zero current.
            //    // 当遥控器掉线的时候，发送给底盘电机零电流.
            //    if (toe_is_error(DBUS_TOE))
            //    {
            //        CAN_cmd_chassis(0, 0, 0, 0);
            //    }
            //    else
            //    {
            //        // send control current
            //        // 发送控制电流
            //        CAN_cmd_chassis(
            //            chassis_move.motor_chassis[0].give_current,
            //            chassis_move.motor_chassis[1].give_current,
            //            chassis_move.motor_chassis[2].give_current,
            //            chassis_move.motor_chassis[3].give_current);
            //    }
            //}
            //// os delay
            //// 系统延时
            // vTaskDelay(CHASSIS_CONTROL_TIME_MS);

            // #if INCLUDE_uxTaskGetStackHighWaterMark
            // chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
            // #endif
            //  std::cout << "Chassis_task \n";
			std::cout << "Chassis task\n";
        }
    }

}  // namespace Chassis