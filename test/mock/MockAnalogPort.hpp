#pragma once

#include "physical_io/IAnalogPort.hpp"
#include <gmock/gmock.h>

class MockAnalogPort : public IAnalogPort {
public:
    MOCK_METHOD(float, read, (), (override));
    MOCK_METHOD(void, write, (float), (override));
    MOCK_METHOD(void, tick, (), (override));
};