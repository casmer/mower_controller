// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef RADIO_CONTROL_RADIO_SWITCH_HPP
#define RADIO_CONTROL_RADIO_SWITCH_HPP

#include <stdint.h>

namespace cotsbotics::radio_control
{
    
    enum class RadioSwitch : uint8_t {
        RS_LOW = 0, ///< Closed (Grounded)
        RS_MIDDLE = 1, ///< Mid (if applicable)
        RS_HIGH = 2 ///< Open (pulled high)
    };

    static inline RadioSwitch fromBool(bool value)
    {
        return value ? RadioSwitch::RS_HIGH : RadioSwitch::RS_LOW;
    }

    static inline bool toBool(RadioSwitch value)
    {
        return value == RadioSwitch::RS_HIGH;
    }
};
#endif // RADIO_CONTROL_RADIO_SWITCH_HPP