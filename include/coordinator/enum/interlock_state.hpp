// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include <stdint.h>

namespace cotsbotics::coordinator
{
    enum class InterlockState : uint8_t
    {
        Disengaged = 0,
        Engaged = 1,
        Fault = 2,
        Unknown = 0xFF
    };

}; // namespace cotsbotics::coordinator