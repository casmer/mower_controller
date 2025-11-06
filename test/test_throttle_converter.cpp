

#include <gmock/gmock.h>

#include <radio_control/throttle_converter.hpp>
#include "radio_control/radio_configuration.hpp"

using namespace cotsbotics::radio_control;
TEST(throttle_converter, convert)
{

    ThrottleConverter uut_throttle_converter(RadioConfiguration::ChannelRanges::MIN_INPUT,
            RadioConfiguration::ChannelRanges::MAX_INPUT,
            RadioConfiguration::ChannelRanges::DEADBAND,
            RadioConfiguration::OutputRanges::MIN_OUTPUT,
            RadioConfiguration::OutputRanges::MAX_OUTPUT);

        {

            int16_t test_value = RadioConfiguration::ChannelRanges::NEUTRAL_INPUT;
            int16_t output_value = uut_throttle_converter.convert(test_value);
            int16_t expected_value = RadioConfiguration::OutputRanges::NEUTRAL_OUTPUT;
            EXPECT_EQ(output_value, expected_value);
        }
        
        {

            int16_t test_value = RadioConfiguration::ChannelRanges::MIN_INPUT;
            int16_t output_value = uut_throttle_converter.convert(test_value);
            int16_t expected_value = RadioConfiguration::OutputRanges::MIN_OUTPUT;
            EXPECT_EQ(output_value, expected_value);
        }

        {

            int16_t test_value = RadioConfiguration::ChannelRanges::MAX_INPUT;
            int16_t output_value = uut_throttle_converter.convert(test_value);
            int16_t expected_value = RadioConfiguration::OutputRanges::MAX_OUTPUT;
            EXPECT_EQ(output_value, expected_value);
        }

        
        {

            int16_t test_value = RadioConfiguration::ChannelRanges::NEUTRAL_INPUT + (RadioConfiguration::ChannelRanges::DEADBAND);
            int16_t output_value = uut_throttle_converter.convert(test_value);
            int16_t expected_value = RadioConfiguration::OutputRanges::NEUTRAL_OUTPUT;
            EXPECT_EQ(output_value, expected_value);
        }

        {

            int16_t test_value = RadioConfiguration::ChannelRanges::NEUTRAL_INPUT - (RadioConfiguration::ChannelRanges::DEADBAND);
            int16_t output_value = uut_throttle_converter.convert(test_value);
            int16_t expected_value = RadioConfiguration::OutputRanges::NEUTRAL_OUTPUT;
            EXPECT_EQ(output_value, expected_value);
        }
        

}