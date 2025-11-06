#pragma once

#include <stdint.h>
#if UNIT_TEST
#include <iostream>
#endif
namespace cotsbotics::radio_control
{
    class ThrottleConverter
    {
    public:

        ThrottleConverter(int32_t min_input, int32_t max_input, int32_t deadband,
                         int32_t min_output, int32_t max_output):
            _min_input(min_input),
            _max_input(max_input),
            _neutral_input((min_input + max_input) / 2),
            _deadband(deadband),
            _min_output(min_output),
            _max_output(max_output),
            _neutral_output((min_output + max_output) / 2)
        {
            #if UNIT_TEST
            std::cout << "ThrottleConverter created with parameters:" << std::endl;
            std::cout << "Min Input: " << min_input << ", Max Input: " << max_input << std::endl;
            std::cout << "Deadband: " << deadband << std::endl;
            std::cout << "Min Output: " << min_output << ", Max Output: " << max_output << std::endl;
            std::cout << "Internal state:" << std::endl;
            std::cout << "Min Input: " << _min_input << ", Max Input: " << _max_input << std::endl;
            std::cout << "Deadband: " << _deadband << std::endl;
            std::cout << "Min Output: " << _min_output << ", Max Output: " << _max_output << std::endl;
            std::cout << "Neutral Input: " << _neutral_input << ", Neutral Output: " << _neutral_output << std::endl;
            #endif
            // Constructor implementation (if needed)
        }
        /// @brief Convert raw throttle input to normalized throttle output
        /// @param raw_input Raw throttle input value
        /// @return Normalized throttle output value
        int32_t convert(int32_t raw_input)
        {
            #if UNIT_TEST
            std::cout << "1 Raw input: " << raw_input << " _min_input: " << _min_input << std::endl;
            #endif
            if (raw_input < _min_input) raw_input = _min_input;
            #if UNIT_TEST
            std::cout << "2. Raw input: " << raw_input << " _max_input: " << _max_input << std::endl;
            #endif
            if (raw_input > _max_input) raw_input = _max_input;
            #if UNIT_TEST
            std::cout << "2. Raw input: " << raw_input << std::endl;
            #endif
            if (raw_input <= (_neutral_input + _deadband) && raw_input >= (_neutral_input - _deadband))
            {
                return _neutral_output;
            }
            return map(raw_input);
        }

    private:
        int32_t _min_input;
        int32_t _max_input;
        int32_t _neutral_input;
        int32_t _deadband;
        int32_t _min_output;
        int32_t _max_output;
        int32_t _neutral_output;
    
        int32_t map(int32_t x)
        {
            return (x - _min_input) * (_max_output - _min_output) / (_max_input - _min_input) + _min_output;
        }
    };
}; // namespace cotsbotics::radio_control