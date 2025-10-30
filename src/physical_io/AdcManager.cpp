
#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/AdcManager.hpp"
#include "millisDelay.h"

namespace cotsbotics::mower_controller
{
    AdcManager::AdcManager(Adafruit_ADS1115 &adc)
        : _adc(adc)
    {
    }

    int AdcManager::readChannel(int channel_num) const
    {
        if (channel_num < MIN_CHANNEL || channel_num >= MAX_CHANNEL)
        {
            return -1;
        }
        return static_cast<int>(_channelValue[channel_num]);
    }

    void AdcManager::setup()
    {
        _current_channel_number = ADS1X15_REG_CONFIG_MUX_SINGLE_0;
        _adc.startADCReading(ADS1X15_REG_CONFIG_MUX_SINGLE_0, true);
        _inputPrintDelay.start(PRINT_DELAY_MS);
        _inputSampleRate.start(INPUT_SAMPLE_RATE_MS);
    }

    void AdcManager::tick()
    {

        if (_inputSampleRate.justFinished())
        {
            int16_t adc_value = _adc.readADC_SingleEnded(static_cast<uint8_t>(_current_channel_number));
            _channelValue[_current_channel_number] = adc_value;
            // Move to next channel
            _current_channel_number++;
            if (_current_channel_number >= MAX_CHANNEL)
            {
                _current_channel_number = 0;
            }
            _inputSampleRate.start(INPUT_SAMPLE_RATE_MS);
        }
        if (_inputPrintDelay.justFinished())
        {
            #ifdef SERIAL_DEBUG
            Serial.print("ADC Channels: ");
            for (uint16_t i = 0; i < MAX_CHANNEL; ++i)
            {
                Serial.print(i);
                Serial.print("-");
                Serial.print(_channelValue[i]);
                Serial.print("          ");
            }
            Serial.println();
            #endif
            _inputPrintDelay.start(PRINT_DELAY_MS);
        }
    }

}; // cotsbotics::mower_controller