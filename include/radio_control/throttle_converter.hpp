#pragma once

#include <stdint.h>

namespace cotsbotics::radio_control
{
    class ThrottleConverter
    {
    public:

        ThrottleConverter(int16_t min_input, int16_t max_input, int16_t deadband,
                         int16_t min_output, int16_t max_output):
            _min_input(min_input),
            _max_input(max_input),
            _neutral_input((min_input + max_input) / 2),
            _deadband(deadband),
            _min_output(min_output),
            _max_output(max_output),
            _neutral_output((min_output + max_output) / 2)
        {
            // Constructor implementation (if needed)
        }
        /// @brief Convert raw throttle input to normalized throttle output
        /// @param raw_input Raw throttle input value
        /// @return Normalized throttle output value
        int16_t convert(int16_t raw_input)
        {
            if (raw_input < _min_input) raw_input = _min_input;
            if (raw_input > _max_input) raw_input = _max_input;
            if (raw_input <= (_neutral_input + _deadband) && raw_input >= (_neutral_input - _deadband))
            {
                return _neutral_output;
            }
            return map(raw_input);
        }

    private:
        int16_t _min_input;
        int16_t _max_input;
        int16_t _neutral_input;
        int16_t _deadband;
        int16_t _min_output;
        int16_t _max_output;
        int16_t _neutral_output;
    
        int16_t map(int16_t x)
        {
            return (x - _min_input) * (_max_output - _min_output) / (_max_input - _min_input) + _min_output;
        }
    };
}; // namespace cotsbotics::radio_control