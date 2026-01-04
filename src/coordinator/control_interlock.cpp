// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "coordinator/control_interlock.hpp"
#if defined(ARDUINO)
#include "Arduino.h"
#define DEBUG_MSG(msg)
#define DEBUG_MSG_A(msg) Serial.println(msg);
#else

#include <iostream>
#define DEBUG_MSG(msg) std::cout << msg << std::endl;
#define DEBUG_MSG_A(msg) std::cout << msg << std::endl;
#endif
namespace cotsbotics
{
    namespace coordinator
    {

        ControlInterlock::ControlInterlock(
            IDigitalInputPort& interlock_a,
            IDigitalInputPort& interlock_b,
            int tick_delay_count) : 
                    _interlock_a(interlock_a),
                    _interlock_b(interlock_b),
                    _tick_delay_count(tick_delay_count)
        {};

        void ControlInterlock::setup()
        {
            _interlock_a.setup();
            _interlock_b.setup();
            // Initialize output signals
        };

        void ControlInterlock::tick()
        {

            _interlock_a.tick();
            _interlock_b.tick();
            _tick_delay_counter++;
            // if (_tick_delay_counter >= _tick_delay_count)
            // {
                _tick_delay_counter = 0;
                determineControlInterlock();
            // }
        };

        #define btos(b) ((b) ? "1" : "0")
        void ControlInterlock::printBuffers()
        {
            #if defined(ARDUINO)
                Serial.print("Interlock A Buffer: ");
            for (int i = 0; i < kInterlockBufferSize; i++)
            {
                Serial.print(btos(_interlock_a_buffer[i]));
                Serial.print(",");
            }
            Serial.println();
            Serial.print("Interlock B Buffer: ");
            for (int i = 0; i < kInterlockBufferSize; i++)
            {
                Serial.print(btos(_interlock_b_buffer[i]));
                Serial.print(",");
            }
            Serial.println();
            #endif
        }
        void ControlInterlock::determineControlInterlock()
        {
            // Logic to determine control mode based on interlock states
            _interlock_a_buffer[_interlock_buffer_index] = _interlock_a.read_raw();
            _interlock_b_buffer[_interlock_buffer_index] = _interlock_b.read_raw();
            DEBUG_MSG("Interlock Index: " << _interlock_buffer_index 
                      << " A: " << _interlock_a_buffer[_interlock_buffer_index]
                      << " B: " << _interlock_b_buffer[_interlock_buffer_index]);
            if (_interlock_buffer_index == kInterlockBufferSize - 1)
            {
                DEBUG_MSG("Interlock signal buffer is now ready.");
                _interlock_signal_ready = true;
            };

            if (!_interlock_signal_ready)
            {
                DEBUG_MSG("Interlock signal buffer not ready yet.");
                // Not enough data yet
                _interlock_state = InterlockState::Unknown;
            }
            else
            {
                // does the current interlock input match the expected values set last tick.


                int interlock_a_sum = 0;
                int interlock_b_sum = 0;
               
                for (int i = 0; i < kInterlockBufferSize; i++)
                {
                    int next_i = (i + 1) % kInterlockBufferSize;
                    
                    DEBUG_MSG("_interlock_a_buffer: [" << _interlock_a_buffer[0] << ", " << _interlock_a_buffer[1] << ", " << _interlock_a_buffer[2] << ", " << _interlock_a_buffer[3] << ", " << _interlock_a_buffer[4] << ", "
                                << _interlock_a_buffer[5] << ", " << _interlock_a_buffer[6] << ", " << _interlock_a_buffer[7] << ", " << _interlock_a_buffer[8] << ", " << _interlock_a_buffer[9] << "]");
                    DEBUG_MSG("_interlock_b_buffer: [" << _interlock_b_buffer[0] << ", " << _interlock_b_buffer[1] << ", " << _interlock_b_buffer[2] << ", " << _interlock_b_buffer[3] << ", " << _interlock_b_buffer[4] << ", "
                                << _interlock_b_buffer[5] << ", " << _interlock_b_buffer[6] << ", " << _interlock_b_buffer[7] << ", " << _interlock_b_buffer[8] << ", " << _interlock_b_buffer[9] << "]");

                    interlock_a_sum += _interlock_a_buffer[i] ? 1 : 0;
                    interlock_b_sum += _interlock_b_buffer[i] ? 1 : 0;
                    
                    
                }

                if ((interlock_a_sum == kInterlockBufferSize) && (interlock_b_sum == 0))
                {
                    DEBUG_MSG("Interlock A High, Interlock B Low. Engaged.");
                    _interlock_state = InterlockState::Engaged;
                }
                else if ((interlock_a_sum == 0) && (interlock_b_sum == kInterlockBufferSize))
                {
                    DEBUG_MSG("Interlock A Low, Interlock B High. Disengaged.");
                    _interlock_state = InterlockState::Disengaged;
                }
                else
                {
                    DEBUG_MSG("Interlock Fault Condition Detected." << " A Sum: " << interlock_a_sum << " B Sum: " << interlock_b_sum);
                    _interlock_state = InterlockState::Fault;
                }
            }
             // Toggle the output signals to indicate interlock status
            _interlock_buffer_index = (_interlock_buffer_index + 1);
            if (_interlock_buffer_index >= kInterlockBufferSize)
            {
                _interlock_buffer_index = 0;
            }
        
        };

        InterlockState ControlInterlock::interlockState() const
        {
            return _interlock_state;
        };

    } // namespace coordinator
} // namespace cotsbotics   