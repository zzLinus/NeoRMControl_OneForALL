#include "chassis/chassis.hpp"

namespace Chassis {
    std::unique_ptr<Mecanum> create_chassis(const MecConfig &config) {
        return std::make_unique<Mecanum>(config);
    }

    std::unique_ptr<Swerve> create_chassis(const SwerveConfig &config) {
        return std::make_unique<Swerve>(config);
    }
}
