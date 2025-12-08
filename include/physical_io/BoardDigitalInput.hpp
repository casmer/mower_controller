// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IDigitalInputPort.hpp"

namespace cotsbotics::mower_controller
{
    class BoardDigitalInput : public IDigitalInputPort
    {
    public:

        explicit BoardDigitalInput(int pin_number);
        /// @brief Virtual destructor
        virtual ~BoardDigitalInput() = default;

        /// @brief Reads the raw digital value from the port without debouncing.
        virtual bool read_raw() const override;

        /// @brief Reads the debounced digital value from the port.
        virtual bool read() const override;

        /// @brief Setup the port
        virtual void setup() override;

        /// @brief Update debounce state (call periodically)
        virtual void tick() override;

        /// @brief Set the debounce delay in number of stable ticks
        /// @param debounce_ticks Number of stable ticks before state is debounced (default 3)
        void set_debounce_delay_ticks(uint32_t debounce_ticks);

    private:
        // Pin number for the digital input
        int _pin_number;

        // Debouncing state
        bool _current_state{false};
        bool _debounced_state{false};
        uint32_t _stable_ticks{0};
        uint32_t _debounce_delay_ticks{3};  // Default 3 stable ticks before debounce

    };

}; // cotsbotics::mower_controller