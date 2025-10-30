

#include <gmock/gmock.h>

#include <radio_control/throttle_converter.hpp>

using namespace cotsbotics::radio_control;
TEST(throttle_converter, convert)
{

    ThrottleConverter uut_throttle_converter(100, 200, 10, 1000, 2000);


    EXPECT_EQ(uut_throttle_converter.convert(100), 1000);
    EXPECT_EQ(uut_throttle_converter.convert(200), 2000);
    EXPECT_EQ(uut_throttle_converter.convert(150), 1500);
    EXPECT_EQ(uut_throttle_converter.convert(140), 1500);
    EXPECT_EQ(uut_throttle_converter.convert(160), 1500);


}