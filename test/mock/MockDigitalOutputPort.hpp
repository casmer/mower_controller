// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IDigitalOutputPort.hpp"

#include <gmock/gmock.h>

namespace cotsbotics::mower_controller
{

class MockDigitalOutputPort : public IDigitalOutputPort 
{
public:
    MOCK_METHOD(void, setup,(), ());
    MOCK_METHOD(void, write, (bool value), ());
};

}; // cotsbotics::mower_controller