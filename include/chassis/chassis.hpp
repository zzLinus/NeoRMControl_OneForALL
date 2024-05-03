#pragma once

#include "chassis/mecanum.hpp"
#include "chassis/swerve.hpp"

namespace Chassis {
    std::unique_ptr<Mecanum> create_chassis(const MecConfig &config);
    std::unique_ptr<Swerve> create_chassis(const SwerveConfig &config);
}
