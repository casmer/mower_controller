// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IAnalogInputPort.hpp"

namespace cotsbotics::mower_controller
{
    class BoardAnalogInput : public IAnalogInputPort
    {
    public:

        explicit BoardAnalogInput(int pin_number);
        /// @brief Virtual destructor
        virtual ~BoardAnalogInput() = default;

        /// @brief Reads the current analog value from the port.
        virtual int read() const override;

        void setup();
    private:
        // Pin number for the digital input
        int _pin_number;
        constexpr static int ANALOG_MAX = 1023; // 10-bit ADC

    };

}; // cotsbotics::mower_controller