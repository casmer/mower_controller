#pragma once

namespace cotsbotics::mower_controller
{

    struct MowerDriveMotorState
    {
        int throttle_position{0};
        bool zero_switch{false};
        int zero_switch_position{-1};
        
        bool operator==(MowerDriveMotorState const &o) const noexcept
        {
            return throttle_position == o.throttle_position
                && zero_switch == o.zero_switch
                && zero_switch_position == o.zero_switch_position;
        }

        bool operator!=(MowerDriveMotorState const &o) const noexcept
        {
            return !(*this == o);
        }
    };

    struct MowerControlState
    {
        /// @brief Left drive motor state
        MowerDriveMotorState left_motor;
        /// @brief Right drive motor state
        MowerDriveMotorState right_motor;
        /// @brief Seat Switch State for Drive Control Module
        bool seat_switch_drive{false};
        /// @brief Seat Switch State for Blade Control Module
        bool seat_switch_blade{false};
        /// @brief Low Speed Drive Enabled
        bool low_speed_drive{false};
        /// @brief Low Speed Cut Enabled
        bool low_speed_cut{false};
        /// @brief Blade Run Enable (PTO)
        bool blades_enabled{false};
        /// @brief Brake Engaged
        bool brake_engaged{false};
        
        bool operator==(MowerControlState const &o) const noexcept
        {
            return left_motor == o.left_motor
                && right_motor == o.right_motor
                && seat_switch_drive == o.seat_switch_drive
                && seat_switch_blade == o.seat_switch_blade
                && low_speed_drive == o.low_speed_drive
                && low_speed_cut == o.low_speed_cut
                && blades_enabled == o.blades_enabled
                && brake_engaged == o.brake_engaged;
        }

        bool operator!=(MowerControlState const &o) const noexcept
        {
            return !(*this == o);
        }
    };

}; // cotsbotics::mower_controller