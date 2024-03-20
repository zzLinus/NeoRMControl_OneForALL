#pragma once

#include "device/deviece_base.hpp"
#include "types.hpp"
#include "bullet_solver.hpp"
#include "robot.hpp"
#include "memory"

namespace Device
{
    class Cv_controller : public DeviceBase
    {
       public:
        explicit Cv_controller() = default;

        void init(const std::shared_ptr<Robot::Robot_set> &robot);
        void unpack(const Robot::ReceiveGimbalPacket &pkg);

       public:
        Control::BulletSolver bullet_solver_;

       private:
        std::shared_ptr<Robot::Robot_set> robot_set;
    };
}  // namespace Device