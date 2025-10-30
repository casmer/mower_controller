// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "sbus/sbus_receiver.hpp"

namespace cotsbotics
{

void SbusReceiver::Begin()
        {
            /* Begin the SBUS communication */
            sbus_rx.Begin();

        }

        void SbusReceiver::tick()
        {
            if (sbus_rx.Read()) {
                /* Grab the received data */
                data = sbus_rx.data();
                m_no_data_received=false;
            }
            else
            {
                m_no_data_received=true;
            }
        };

}; // cotsbotics
