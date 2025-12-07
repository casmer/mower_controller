// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

// Demo for configuring the Vref of the MCP4728 4-Channel 12-bit I2C DAC
#include <Adafruit_MCP4728.h>

#include "mower_controller/pin_definitions.hpp"
#include "physical_io/IAnalogOutputPort.hpp"

namespace cotsbotics::mower_controller
{
    class DacAnalogOutput : public IAnalogOutputPort
    {
    public:
        /// @brief Maximum DAC value    
        static constexpr uint16_t MAX_DAC_VALUE = 4095;
        /// @brief Minimum DAC value
        static constexpr uint16_t MIN_DAC_VALUE = 0;


        explicit DacAnalogOutput(analogPin_t pinNumber, Adafruit_MCP4728& mcp);
        /// @brief Virtual destructor
        virtual ~DacAnalogOutput() = default;

        /// @brief Set the digital output value (true = high, false = low)
        /// @param value The value to set
        virtual void write(int value) override;

        void setup();
    private:
        // Pin number for the digital input
        analogPin_t _channel;
        Adafruit_MCP4728& _mcp;

    };

}; // cotsbotics::mower_controller