#pragma once

#include <stdint.h>
#define CH(x) (x-1)
namespace cotsbotics::radio_control
{
    class RadioConfiguration
    {
    public:
        class Channels
        {
        public:
            static int8_t constexpr RIGHT_STICK_X = CH(1);
            static int8_t constexpr RIGHT_STICK_Y = CH(2);
            static int8_t constexpr LEFT_STICK_X = CH(4);
            static int8_t constexpr LEFT_STICK_Y = CH(3);

            static int8_t constexpr ZERO_SWITCH = CH(5);
            static int8_t constexpr BLADE_ENABLE = CH(10);
            static int8_t constexpr LOW_SPEED_DRIVE = CH(12);
            static int8_t constexpr LOW_SPEED_CUT = CH(9);

            static int8_t constexpr SEAT_SWITCH = CH(11);

            
        };

        class  ChannelRanges
        {
        public:
            static int32_t constexpr MIN_INPUT = 325;
            static int32_t constexpr MAX_INPUT = 1690;
            static int32_t constexpr NEUTRAL_INPUT = (MIN_INPUT + MAX_INPUT) / 2;
            static int32_t constexpr DEADBAND = 50;
        };

        class  OutputRanges
        {
        public:
            static int32_t constexpr MIN_OUTPUT = 0;
            static int32_t constexpr MAX_OUTPUT = 4095;
            static int32_t constexpr NEUTRAL_OUTPUT = (MIN_OUTPUT + MAX_OUTPUT) / 2;
        };

    };
}; // namespace cotsbotics::mower_controller