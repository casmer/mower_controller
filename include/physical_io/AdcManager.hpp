// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include <Adafruit_ADS1X15.h>
#include "millisDelay.h"

namespace cotsbotics::mower_controller
{
    class AdcManager 
    {
    public:
        /// @brief constructor
        /// @param adc instance of Adafruit ADS1115 ADC
        explicit AdcManager(Adafruit_ADS1115& adc);
        /// @brief Virtual destructor
        ~AdcManager() = default;

        /// @brief Reads the current analog value from the port.
        int readChannel(int channel_num) const;

        void setup();
        void tick();

        constexpr static int MAX_CHANNEL = 4; 

        constexpr static int MIN_CHANNEL = 0; 
    private:
        /// @brief Adafruit ADS1115 ADC instance
        Adafruit_ADS1115& _adc; 
        int _current_channel_number;
        int16_t _channelValue[4]{0,0,0,0};
        millisDelay _inputPrintDelay;
        millisDelay _inputSampleRate;;
        static constexpr unsigned long INPUT_SAMPLE_RATE_MS = 1;
        static constexpr unsigned long PRINT_DELAY_MS = 800;
    };

}; // cotsbotics::mower_controller