// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>


#include <gmock/gmock.h>

#include <coordinator/control_interlock.hpp>

#include <mock/MockDigitalOutputPort.hpp>
#include <mock/MockDigitalInputPort.hpp>
#include "mower_controller/mower_manual_control_input.hpp"

std::string interlockStateToString(cotsbotics::coordinator::InterlockState state)
{
    switch (state)
    {
    case cotsbotics::coordinator::InterlockState::Disengaged:
        return "Disengaged [0]";
    case cotsbotics::coordinator::InterlockState::Engaged:
        return "Engaged [1]";
    case cotsbotics::coordinator::InterlockState::Fault:
        return "Fault [2]";
    case cotsbotics::coordinator::InterlockState::Unknown:
        return "Unknown [0xFF]";
    default:
        return "Invalid State";
    }
}

namespace cotsbotics::coordinator
{


void PrintTo(const cotsbotics::coordinator::InterlockState& state, std::ostream* os)
{
    *os << interlockStateToString(state);
};

TEST(control_interlock, initialize_disengaged)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b);

    bool expected_interlock_a = false;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        

        control_interlock.tick();
        std::cout << "Interlock State: " << interlockStateToString(control_interlock.interlockState()) << std::endl;

        if (i == ControlInterlock::kInterlockBufferSize - 1)
        {
            interlock_signal_ready = true;
        };

        if (interlock_signal_ready)
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Disengaged);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), true);
        }
        else
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Unknown);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), false);
        };

    }

};




TEST(control_interlock, initialize_engaged)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b);

    bool expected_interlock_a = true;
    bool expected_interlock_b = false;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        

        control_interlock.tick();
        std::cout << "Interlock State: " << interlockStateToString(control_interlock.interlockState()) << std::endl;

        if (i == ControlInterlock::kInterlockBufferSize - 1)
        {
            interlock_signal_ready = true;
        };

        if (interlock_signal_ready)
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Engaged);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), true);
        }
        else
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Unknown);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), false);
        };

    }
};

    
TEST(control_interlock, initialize_fault_shorted)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b);

    bool expected_interlock_a = false;
    bool expected_interlock_b = false;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        

        control_interlock.tick();
        std::cout << "Interlock State: " << interlockStateToString(control_interlock.interlockState()) << std::endl;

        if (i == ControlInterlock::kInterlockBufferSize - 1)
        {
            interlock_signal_ready = true;
        };

        if (interlock_signal_ready)
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Fault);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), true);
        }
        else
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Unknown);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), false);
        };

    }

};


TEST(control_interlock, initialize_fault_broken)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b);

    bool expected_interlock_a = true;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        

        control_interlock.tick();
        std::cout << "Interlock State: " << interlockStateToString(control_interlock.interlockState()) << std::endl;

        if (i == ControlInterlock::kInterlockBufferSize - 1)
        {
            interlock_signal_ready = true;
        };

        if (interlock_signal_ready)
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Fault);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), true);
        }
        else
        {
            EXPECT_EQ(control_interlock.interlockState(), InterlockState::Unknown);
            EXPECT_EQ(control_interlock.isInterlockSignalReady(), false);
        };

    }

};



} // namespace cotsbotics::coordinator
