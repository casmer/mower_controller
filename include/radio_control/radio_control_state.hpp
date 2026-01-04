// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "radio_control/radio_switch.hpp"
#include "radio_control/radio_configuration.hpp"


namespace cotsbotics::radio_control
{

    struct RadioControlState
    {
        /// @brief Left throttle position 
        int left_throttle_position{RadioConfiguration::OutputRanges::NEUTRAL_OUTPUT};
        /// @brief Right throttle position
        int right_throttle_position{RadioConfiguration::OutputRanges::NEUTRAL_OUTPUT};
        /// @brief Zero switch state
        RadioSwitch zero_switch{RadioSwitch::RS_LOW};
        
        /// @brief Seat Switch State for Drive Control Module
        RadioSwitch seat_switch_drive{RadioSwitch::RS_LOW};
        /// @brief Seat Switch State for Blade Control Module
        RadioSwitch seat_switch_blade{RadioSwitch::RS_LOW};
        /// @brief Low Speed Drive Enabled
        RadioSwitch low_speed_drive{RadioSwitch::RS_LOW};
        /// @brief Low Speed Cut Enabled
        RadioSwitch low_speed_cut{RadioSwitch::RS_LOW};
        /// @brief Blade Run Enable (PTO)
        RadioSwitch blades_enabled{RadioSwitch::RS_LOW};
        /// @brief Control Mode Selection
        RadioSwitch control_mode{RadioSwitch::RS_LOW};
        /// @brief Failsafe status - true if in failsafe condition (no valid signal)
        bool failsafe{true};
        
        bool operator==(RadioControlState const &o) const noexcept
        {
            return left_throttle_position == o.left_throttle_position
                && right_throttle_position == o.right_throttle_position
                && zero_switch == o.zero_switch
                && seat_switch_drive == o.seat_switch_drive
                && seat_switch_blade == o.seat_switch_blade
                && low_speed_drive == o.low_speed_drive
                && low_speed_cut == o.low_speed_cut
                && blades_enabled == o.blades_enabled
                && control_mode == o.control_mode
                && failsafe == o.failsafe;
        }

        bool operator!=(RadioControlState const &o) const noexcept
        {
            return !(*this == o);
        }
    };

}; // cotsbotics::mower_controller