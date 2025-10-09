#pragma once

#include "physical_io/IAnalogInputPort.hpp"

#include <gmock/gmock.h>

namespace cotsbotics::mower_controller
{

class MockAnalogInputPort : public IAnalogInputPort {
public:
    MOCK_METHOD(int, read,(), (const));
    MOCK_METHOD(void, setup,(), ());
};

}; // cotsbotics::mower_controller