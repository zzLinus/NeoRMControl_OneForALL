#include "device/imu.hpp"

#include "hardware.hpp"
#include "iostream"

namespace Device
{
    void IMU::unpack(const Types::ReceivePacket &pkg) {
        robot_set->ins_yaw = pkg.yaw;
        robot_set->ins_pitch = -pkg.pitch;
        robot_set->ins_roll = pkg.roll;
        robot_set->ins_yaw_v = pkg.yaw_v;
        robot_set->ins_pitch_v = pkg.pitch_v;
        robot_set->ins_roll_v = -pkg.roll_v;
        update_time();
    }

    void IMU::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        Robot::hardware->register_callback<SER1>([&](const Types::ReceivePacket &rp) {
//            LOG_INFO("%f, %f, %f\n", rp.roll, rp.pitch, rp.yaw);

            unpack(rp);

            Robot::SendVisionControl svp;
            svp.header = 0xA6;
            svp.yaw = robot_set->ins_yaw;
            svp.pitch = robot_set->ins_pitch;
            svp.roll = robot_set->ins_roll;
            Robot::hardware->send<SOCKET>(svp);

            Robot::SendGimbalPacket sgp;
            sgp.detect_color = 1;
            sgp.reserved = 0;
            sgp.reset_tracker = false;
            sgp.header = 0x5A;
            sgp.yaw = robot_set->ins_yaw;
            sgp.pitch = -robot_set->ins_pitch;
            sgp.roll = robot_set->ins_roll;
            Robot::hardware->send<SOCKET>(sgp);
        });
    }
}  // namespace Device
