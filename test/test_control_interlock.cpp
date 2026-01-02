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
    MockDigitalOutputPort interlock_a2;
    MockDigitalOutputPort interlock_b2;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b,
        interlock_a2,
        interlock_b2);

    bool expected_interlock_a = false;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);
    EXPECT_CALL(interlock_a2, setup()).Times(1);
    EXPECT_CALL(interlock_b2, setup()).Times(1);
    EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(1);
    EXPECT_CALL(interlock_b2, write(expected_interlock_b)).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        std::cout << "---- Tick Iteration " << i << " ----" << std::endl;
        std::cout << " expected_interlock_a: " << expected_interlock_a << std::endl;
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        
        EXPECT_CALL(interlock_a2, write(!expected_interlock_a)).Times(1);
        EXPECT_CALL(interlock_b2, write(expected_interlock_a)).Times(1);

        // EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(0);
        // EXPECT_CALL(interlock_b2, write(!expected_interlock_a)).Times(0);

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

        expected_interlock_a= !expected_interlock_a;

    }

};



TEST(control_interlock, initialize_engaged)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;
    MockDigitalOutputPort interlock_a2;
    MockDigitalOutputPort interlock_b2;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b,
        interlock_a2,
        interlock_b2);

    //default values for the write to the DIO ports during setup
    bool expected_interlock_a = false;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);
    EXPECT_CALL(interlock_a2, setup()).Times(1);
    EXPECT_CALL(interlock_b2, setup()).Times(1);
    EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(1);
    EXPECT_CALL(interlock_b2, write(expected_interlock_b)).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;
    //should be high all the time for engaged
    expected_interlock_a = true;
    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        std::cout << "---- Tick Iteration " << i << " ----" << std::endl;
        std::cout << " expected_interlock_a: " << expected_interlock_a << std::endl;
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        
        EXPECT_CALL(interlock_a2, write(expected_interlock_b)).Times(1);
        EXPECT_CALL(interlock_b2, write(!expected_interlock_b)).Times(1);


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

        expected_interlock_b= !expected_interlock_b;

    }

};

TEST(control_interlock, bbad_signal)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;
    MockDigitalOutputPort interlock_a2;
    MockDigitalOutputPort interlock_b2;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b,
        interlock_a2,
        interlock_b2);

    bool expected_interlock_a = false;

    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);
    EXPECT_CALL(interlock_a2, setup()).Times(1);
    EXPECT_CALL(interlock_b2, setup()).Times(1);
    EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(1);
    EXPECT_CALL(interlock_b2, write(!expected_interlock_a)).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        std::cout << "---- Tick Iteration " << i << " ----" << std::endl;
        std::cout << " expected_interlock_a: " << expected_interlock_a << std::endl;
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        
        EXPECT_CALL(interlock_a2, write(!expected_interlock_a)).Times(1);
        EXPECT_CALL(interlock_b2, write(expected_interlock_a)).Times(1);

        // EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(0);
        // EXPECT_CALL(interlock_b2, write(!expected_interlock_a)).Times(0);

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
        }

        expected_interlock_a= !expected_interlock_a;

    }

}



TEST(control_interlock, initialize_disengaged_bad_a_signal_short)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;
    MockDigitalOutputPort interlock_a2;
    MockDigitalOutputPort interlock_b2;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b,
        interlock_a2,
        interlock_b2);

    bool expected_interlock_a = false;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);
    EXPECT_CALL(interlock_a2, setup()).Times(1);
    EXPECT_CALL(interlock_b2, setup()).Times(1);
    EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(1);
    EXPECT_CALL(interlock_b2, write(expected_interlock_b)).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    bool bad_a_signal_introduced = false;
    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        std::cout << "---- Tick Iteration " << i << " ----" << std::endl;
        std::cout << " expected_interlock_a: " << expected_interlock_a << std::endl;
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(bad_a_signal_introduced));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        
        EXPECT_CALL(interlock_a2, write(!expected_interlock_a)).Times(1);
        EXPECT_CALL(interlock_b2, write(expected_interlock_a)).Times(1);

        // EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(0);
        // EXPECT_CALL(interlock_b2, write(!expected_interlock_a)).Times(0);

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

        expected_interlock_a= !expected_interlock_a;

    }

};




TEST(control_interlock, initialize_disengaged_bad_a_signal_broken)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;
    MockDigitalOutputPort interlock_a2;
    MockDigitalOutputPort interlock_b2;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b,
        interlock_a2,
        interlock_b2);

    bool expected_interlock_a = false;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);
    EXPECT_CALL(interlock_a2, setup()).Times(1);
    EXPECT_CALL(interlock_b2, setup()).Times(1);
    EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(1);
    EXPECT_CALL(interlock_b2, write(expected_interlock_b)).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    bool bad_a_signal_introduced = true;
    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        std::cout << "---- Tick Iteration " << i << " ----" << std::endl;
        std::cout << " expected_interlock_a: " << expected_interlock_a << std::endl;
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(bad_a_signal_introduced));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(expected_interlock_b));
        
        EXPECT_CALL(interlock_a2, write(!expected_interlock_a)).Times(1);
        EXPECT_CALL(interlock_b2, write(expected_interlock_a)).Times(1);

        // EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(0);
        // EXPECT_CALL(interlock_b2, write(!expected_interlock_a)).Times(0);

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

        expected_interlock_a= !expected_interlock_a;

    }

};


TEST(control_interlock, initialize_disengaged_bad_b_signal_short)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;
    MockDigitalOutputPort interlock_a2;
    MockDigitalOutputPort interlock_b2;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b,
        interlock_a2,
        interlock_b2);

    bool expected_interlock_a = false;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);
    EXPECT_CALL(interlock_a2, setup()).Times(1);
    EXPECT_CALL(interlock_b2, setup()).Times(1);
    EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(1);
    EXPECT_CALL(interlock_b2, write(expected_interlock_b)).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    bool bad_b_signal_introduced = false;
    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        std::cout << "---- Tick Iteration " << i << " ----" << std::endl;
        std::cout << " expected_interlock_a: " << expected_interlock_a << std::endl;
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(bad_b_signal_introduced));
        
        EXPECT_CALL(interlock_a2, write(!expected_interlock_a)).Times(1);
        EXPECT_CALL(interlock_b2, write(expected_interlock_a)).Times(1);

        // EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(0);
        // EXPECT_CALL(interlock_b2, write(!expected_interlock_a)).Times(0);

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

        expected_interlock_a= !expected_interlock_a;

    }

};


TEST(control_interlock, initialize_disengaged_bad_b_signal_broken)
{

    MockDigitalInputPort interlock_a;
    MockDigitalInputPort interlock_b;
    MockDigitalOutputPort interlock_a2;
    MockDigitalOutputPort interlock_b2;

    ControlInterlock control_interlock(
        interlock_a,
        interlock_b,
        interlock_a2,
        interlock_b2);

    bool expected_interlock_a = false;
    bool expected_interlock_b = true;
    EXPECT_CALL(interlock_a, setup()).Times(1);
    EXPECT_CALL(interlock_b, setup()).Times(1);
    EXPECT_CALL(interlock_a2, setup()).Times(1);
    EXPECT_CALL(interlock_b2, setup()).Times(1);
    EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(1);
    EXPECT_CALL(interlock_b2, write(expected_interlock_b)).Times(1);

    control_interlock.setup();
    bool interlock_signal_ready = false;

    bool bad_b_signal_introduced = true;
    for (int i = 0; i < ControlInterlock::kInterlockBufferSize; i++)
    {
        std::cout << "---- Tick Iteration " << i << " ----" << std::endl;
        std::cout << " expected_interlock_a: " << expected_interlock_a << std::endl;
        SCOPED_TRACE("Iteration " + std::to_string(i));


        EXPECT_CALL(interlock_a, tick()).Times(1);
        EXPECT_CALL(interlock_b, tick()).Times(1);

        EXPECT_CALL(interlock_a, read()).Times(1).WillOnce(testing::Return(expected_interlock_a));
        EXPECT_CALL(interlock_b, read()).Times(1).WillOnce(testing::Return(bad_b_signal_introduced));
        
        EXPECT_CALL(interlock_a2, write(!expected_interlock_a)).Times(1);
        EXPECT_CALL(interlock_b2, write(expected_interlock_a)).Times(1);

        // EXPECT_CALL(interlock_a2, write(expected_interlock_a)).Times(0);
        // EXPECT_CALL(interlock_b2, write(!expected_interlock_a)).Times(0);

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

        expected_interlock_a= !expected_interlock_a;

    }

};



} // namespace cotsbotics::coordinator
