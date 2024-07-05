#pragma once
#include <memory>
#include <thread>

#include "can.hpp"
#include "chassis.hpp"
#include "config.hpp"
#include "gimbal.hpp"
#include "hardware.hpp"
#include "rc_ctrl.hpp"
#include "robot.hpp"
#include "serial_interface.hpp"
#include "shoot.hpp"
#include "socket_interface.hpp"
#include "device/imu.hpp"
#include "device/cv_controller.hpp"

namespace Robot
{

    class Robot_ctrl
    {
       public:
        Robot_ctrl();
        ~Robot_ctrl();

        void load_hardware();
        void start_init();
        void init_join() const;
        void start();
        void join() const;

       public:
        std::unique_ptr<std::thread> chassis_thread;
        std::unique_ptr<std::thread> gimbal_thread;
        std::unique_ptr<std::thread> vision_thread;
        std::unique_ptr<std::thread> shoot_thread;
        std::unique_ptr<std::thread> gimbal_init_thread;

        std::shared_ptr<Robot_set> robot_set;

        Device::IMU imu;
        Device::Cv_controller cv_controller_;
        Chassis::Chassis chassis;
        Gimbal::Gimbal gimbal;
        Shoot::Shoot shoot;

        Hardware::Serial_interface<Types::ReceivePacket> *ser1;
        Io::Server_socket_interface *socket_intrf;
        Io::Rc_ctrl rc_ctrl;

       private:
    };

}  // namespace Robot
