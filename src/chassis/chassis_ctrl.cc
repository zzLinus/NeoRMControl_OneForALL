#include "chassis_ctrl.hpp"

namespace Chassis
{

    Chassis_ctrl::Chassis_ctrl()
    {
        // TODO: move all pid configureation to a config file and load it in when init
        for (auto& m : mecanum_wheel)
        {
            m = new Hardware::Motor(1.0, 1.0, 1.0);
        }
    }

    Chassis_ctrl::~Chassis_ctrl()
    {
        for (auto& m : mecanum_wheel)
        {
            delete m;
        }
    }

    void Chassis_ctrl::init()
    {
    }

}  // namespace Chassis
