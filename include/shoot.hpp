#include "config.hpp"
#include "hardware.hpp"
#include "motor.hpp"
#include "pid_controller.hpp"
#include "robot.hpp"
#include "ramp.hpp"

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
        void control_loop();
        bool isJam();

       public:
        bool no_force = true;
        bool friction_open = false;
        bool friction_finish = false;
        bool shoot_open = false;

        int jam_time = 0;
        int back_time = 0;

        std::vector<Hardware::Motor> friction;
        std::vector<Hardware::Motor> trigger;

        std::shared_ptr<Robot::Robot_set> robot_set;
       private:
        UserLib::Ramp friction_ramp;
    };
}  // namespace Shoot