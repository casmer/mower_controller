#pragma once

#include "physical_io/IDigitalPort.hpp"

#include <gmock/gmock.h>

namespace cotsbotics::mower_controller
{

class MockDigitalPort : public IDigitalPort {
public:
    MOCK_METHOD(bool, read, (), (const));
    MOCK_METHOD(void, write, (bool));
    MOCK_METHOD(void, tick, ());
};
}; // cotsbotics::mower_controller