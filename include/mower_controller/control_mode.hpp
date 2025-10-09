// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef MOWER_CONTROL_MODE_HPP
#define MOWER_CONTROL_MODE_HPP

#include <stdint.h>

namespace cotsbotics::mower_controller
{
enum class ControlMode : uint8_t{
    Manual = 0, ///< Manual Control (e.g. controls on mower with driver in seat.)
    Robotic = 1 ///< Robotic Control (e.g. remote control or autonomous operation)
};
};
#endif // MOWER_CONTROL_MODE_HPP