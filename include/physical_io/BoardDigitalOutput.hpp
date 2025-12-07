// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IDigitalOutputPort.hpp"

namespace cotsbotics::mower_controller
{
    class BoardDigitalOutput : public IDigitalOutputPort
    {
    public:

        explicit BoardDigitalOutput(int pinNumber);
        /// @brief Virtual destructor
        virtual ~BoardDigitalOutput() = default;

        /// @brief Set the digital output value (true = high, false = low)
        /// @param value The value to set
        virtual void write(bool value) override;

        void setup();
    private:
        // Pin number for the digital input
        int _pinNumber;

    };

}; // cotsbotics::mower_controller