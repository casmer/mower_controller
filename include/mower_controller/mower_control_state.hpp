#pragma once

namespace cotsbotics::mower_controller
{

    struct MowerDriveMotorState
    {
        float throtle_position{0.0f};
        bool zero_switch{false};
        float zero_switch_position{0.0f};
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
    };

}; // cotsbotics::mower_controller