#pragma once

#include "physical_io/IDigitalInputPort.hpp"

#include <gmock/gmock.h>

namespace cotsbotics::mower_controller
{

class MockDigitalInputPort : public IDigitalInputPort {
public:
    MOCK_METHOD(bool, read, (), (const));
    MOCK_METHOD(void, setup,(), ());
};
}; // cotsbotics::mower_controller