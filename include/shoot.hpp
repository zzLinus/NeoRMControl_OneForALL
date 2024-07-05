#include "config.hpp"
#include "hardware.hpp"
#include "motor.hpp"
#include "pid_controller.hpp"
#include "robot.hpp"
#include "ramp.hpp"
#include "motor_sender.hpp"

namespace Shoot
{
    class Shoot
    {
       public:
        Shoot();
        void init(const std::shared_ptr<Robot::Robot_set> &robot);
        ~Shoot() = default;
        void update_speed();
        void decomposition_speed();
        [[noreturn]] void task();
        bool isJam();

       public:
        bool friction_finish = false;

        int jam_time = 0;
        int back_time = 0;

        std::vector<Hardware::Motor> friction;
        std::vector<Hardware::Motor> trigger;
        Hardware::Motor_sender friction_sender;
        Hardware::Motor_sender trigger_sender;

        std::shared_ptr<Robot::Robot_set> robot_set;
       private:
        UserLib::Ramp friction_ramp;
    };
}  // namespace Shoot