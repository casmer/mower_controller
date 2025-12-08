// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "coordinator/fault_configuration.hpp"

namespace cotsbotics::coordinator
{
    const FaultConfig& FaultConfiguration::get_fault_config(FaultCode fault_code)
    {
        uint8_t index = fault_code_to_index(fault_code);
        if (index >= FAULT_CONFIGS.size())
        {
            // Return NO_FAULT config as a safe default
            return FAULT_CONFIGS[static_cast<uint8_t>(FaultCode::NO_FAULT)];
        }
        return FAULT_CONFIGS[index];
    }

    const etl::array<FaultConfig, static_cast<uint8_t>(FaultCode::MAX_FAULT_CODE)>&
    FaultConfiguration::get_all_configs()
    {
        return FAULT_CONFIGS;
    }

    uint8_t FaultConfiguration::fault_code_to_index(FaultCode fault_code)
    {
        return static_cast<uint8_t>(fault_code);
    }

}  // namespace cotsbotics::coordinator
