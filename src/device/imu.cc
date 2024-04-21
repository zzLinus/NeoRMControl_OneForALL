#include "device/imu.hpp"

#include "hardware.hpp"

namespace Device
{
    void IMU::unpack(const Types::ReceivePacket &pkg) {
        robot_set->ins_yaw = pkg.yaw;
        robot_set->ins_pitch = -pkg.pitch;
        robot_set->ins_roll = pkg.roll;
        robot_set->ins_yaw_v = -pkg.yaw_v;
        robot_set->ins_pitch_v = -pkg.pitch_v;
        robot_set->ins_roll_v = -pkg.roll_v;
        update_time();
    }

    void IMU::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        Robot::io<SER>[0]->register_callback([&](const Types::ReceivePacket &rp) {
            unpack(rp);
            Robot::SendVisionControl svp;
            svp.header = 0xA6;
            svp.yaw = robot_set->ins_yaw;
            svp.pitch = robot_set->ins_pitch;
            svp.roll = robot_set->ins_roll;
            Robot::io<SOCKET>[0]->send(svp);

            Robot::SendGimbalPacket sgp;
            sgp.header = 0x5A;
            sgp.yaw = robot_set->ins_yaw;
            sgp.pitch = robot_set->ins_pitch;
            sgp.roll = robot_set->ins_roll;
            Robot::io<SOCKET>[0]->send(sgp);
        });
    }
}  // namespace Device
