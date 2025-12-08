// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IDigitalInputPort.hpp"

#include <gmock/gmock.h>

namespace cotsbotics::mower_controller
{

class MockDigitalInputPort : public IDigitalInputPort {
public:
    MOCK_METHOD(bool, read_raw, (), (const));
    MOCK_METHOD(bool, read, (), (const));
    MOCK_METHOD(void, setup,(), ());
    MOCK_METHOD(void, tick, ());
};
}; // cotsbotics::mower_controller