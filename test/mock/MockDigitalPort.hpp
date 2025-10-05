#pragma once

#include "physical_io/IDigitalPort.hpp"
#include <gmock/gmock.h>

class MockDigitalPort : public IDigitalPort {
public:
    MOCK_METHOD(float, read, (), (override));
    MOCK_METHOD(void, write, (float), (override));
    MOCK_METHOD(void, tick, (), (override));
};