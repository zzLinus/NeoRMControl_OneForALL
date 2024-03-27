#include "device/cv_controller.hpp"

#include "hardware.hpp"
#include "utils.hpp"

namespace Device
{
    void Cv_controller::init(const std::shared_ptr<Robot::Robot_set>& robot) {
        robot_set = robot;
        Robot::hardware->register_callback<SOCKET, Robot::ReceiveGimbalPacket>(
            [&](const Robot::ReceiveGimbalPacket& pkg) { unpack(pkg); });
    }

    [[noreturn]] void Cv_controller::task() {
        while (true) {
            Robot::SendGimbalPacket sgp;
            sgp.detect_color = 1;
            sgp.reserved = 0;
            sgp.reset_tracker = false;
            sgp.header = 0x5A;
            sgp.yaw = 0.f;
            sgp.pitch = robot_set->ins_pitch;
            sgp.roll = robot_set->ins_roll;
            sgp.aim_x = robot_set->aimx;
            sgp.aim_y = robot_set->aimy;
            sgp.aim_z = robot_set->aimz;
            Robot::hardware->send<SOCKET>(sgp);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    void Cv_controller::unpack(const Robot::ReceiveGimbalPacket& pkg) {
        if (pkg.x != 0) {
            auto solver_successful = bullet_solver_.solve(
                { pkg.x, pkg.y, pkg.z },
                { pkg.vx, pkg.vy, pkg.vz },
                17,
                pkg.yaw,
                pkg.v_yaw,
                pkg.r1,
                pkg.r2,
                pkg.dz,
                pkg.armors_num);
            LOG_INFO("yaw: %f, pitch: %f, roll: %f\n", robot_set->ins_roll, robot_set->ins_pitch, robot_set->ins_yaw);
            if (solver_successful) {
                robot_set->yaw_set = (fp32)bullet_solver_.getYaw();
                robot_set->pitch_set = (fp32)bullet_solver_.getPitch();
                robot_set->aimx = (fp32)bullet_solver_.target_pos_.x;
                robot_set->aimy = (fp32)bullet_solver_.target_pos_.y;
                robot_set->aimz = (fp32)bullet_solver_.target_pos_.z;
            }
        }
        update_time();
    }

}  // namespace Device
