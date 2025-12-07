// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IAnalogInputPort.hpp"
#include "physical_io/AdcManager.hpp"

namespace cotsbotics::mower_controller
{
    class AdcAnalogInput : public IAnalogInputPort
    {
    public:

        explicit AdcAnalogInput(int pin_number, AdcManager& adc_manager);
        /// @brief Virtual destructor
        virtual ~AdcAnalogInput() = default;

        /// @brief Reads the current analog value from the port.
        virtual int read() const override;

        void setup();

    private:
        // Pin number for the digital input
        uint16_t _pin_number;;
        AdcManager& _adc_manager; 
    };

}; // cotsbotics::mower_controller