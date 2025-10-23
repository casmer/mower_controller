// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef MOWER_MOWER_SWITCH_HPP
#define MOWER_MOWER_SWITCH_HPP

#include <stdint.h>

namespace cotsbotics::mower_controller
{
    
    enum class MowerSwitch : uint8_t{
        CLOSED = 0, ///< Closed (Grounded)
        OPEN = 1 ///< Open (pulled high)
    };

    static inline MowerSwitch fromBool(bool value)
    {
        return value ? MowerSwitch::OPEN : MowerSwitch::CLOSED;
    }

    static inline bool toBool(MowerSwitch value)
    {
        return value == MowerSwitch::OPEN;
    }
};
#endif // MOWER_MOWER_SWITCH_HPP