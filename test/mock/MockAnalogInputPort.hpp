#pragma once

#include "physical_io/IAnalogInputPort.hpp"

#include <gmock/gmock.h>

namespace cotsbotics::mower_controller
{

class MockAnalogInputPort : public IAnalogInputPort {
public:
    MOCK_METHOD(float, read,(), (const));
};

}; // cotsbotics::mower_controller