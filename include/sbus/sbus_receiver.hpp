// MIT License
//  Copyright (c) 2025 Cotsbotics
//  Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef SBUS_SBUS_RECEIVER_HPP
#define SBUS_SBUS_RECEIVER_HPP

#include "sbus.h"
#include "millisDelay.h"

namespace cotsbotics
{
    class SbusReceiver
    {
    public:
        SbusReceiver()
        {
        }

        bool no_data() { return m_no_data_received; };
        void Begin();

        void tick();

        uint16_t get_channel(int8_t channel)
        {
            if (channel < 0 || channel >= data.NUM_CH)
                return 0;
            return data.ch[channel];
        };

        bool get_lost_frame() { return data.lost_frame; };
        bool get_failsafe() { return data.failsafe; };

    private:
        bool m_no_data_received{true};
        /* SBUS object, reading SBUS */
        bfs::SbusRx sbus_rx{&Serial2, true};
        /* SBUS data */
        bfs::SbusData data;
        // output current state data
        millisDelay printDelay;

        static constexpr unsigned long PRINT_DELAY_MS = 40;
    };
}; // cotsbotics

#endif // SBUS_SBUS_RECEIVER_HPP