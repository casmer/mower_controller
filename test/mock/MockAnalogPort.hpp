#pragma once

#include "physical_io/IAnalogPort.hpp"

#include <gmock/gmock.h>

namespace cotsbotics::mower_controller
{

class MockAnalogPort : public IAnalogPort {
public:
    MOCK_METHOD(float, read,(), (const));
    MOCK_METHOD(void, write, (float));
    MOCK_METHOD(void, tick, ());
};

}; // cotsbotics::mower_controller