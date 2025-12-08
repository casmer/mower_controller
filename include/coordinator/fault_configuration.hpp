// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once
#define ETL_NO_STL

#include "coordinator/fault_manager.hpp"
#include <stdint.h>
#include <etl/array.h>

namespace cotsbotics::coordinator
{
    /// @brief Configuration thresholds for fault maturity and recovery
    struct FaultConfig
    {
        constexpr FaultConfig(uint8_t maturity_count, uint8_t recovery_count)
            : maturity_count(maturity_count),
              recovery_count(recovery_count)
        {
        }
        /// @brief number of ticks a fault must persist to be considered "mature"
        uint8_t maturity_count{1};

        /// @brief number of ticks required for recovery/resolution of a fault
        uint8_t recovery_count{1};
    };

    /// @brief Provides fault configuration by fault ID
    class FaultConfiguration
    {
    public:
        /// @brief Get the configuration for a specific fault
        /// @param fault_code The fault to query
        /// @return Reference to the fault configuration
        static const FaultConfig& get_fault_config(FaultCode fault_code);

        /// @brief Get all fault configurations as an array
        /// @return Reference to the complete configuration array
        static const etl::array<FaultConfig, static_cast<uint8_t>(FaultCode::MAX_FAULT_CODE)>&
        get_all_configs();

    private:
        typedef etl::array<FaultConfig, static_cast<uint8_t> (FaultCode::MAX_FAULT_CODE)> FaultConfigArray;
        /// @brief Array of fault configurations indexed by FaultCode
        static constexpr FaultConfigArray FAULT_CONFIGS{{
            FaultConfig(0, 0), // NO_FAULT (0)
            FaultConfig(10, 100), // INTERLOC_MISMATCH_ERROR (1)
            FaultConfig(5, 1), // SBUS_RECEIVER_ERROR (2)
            FaultConfig(6, 20), // RADIO_LOSS_OF_SIGNAL (3) 
        }};

        /// @brief Convert FaultCode to array index
        static uint8_t fault_code_to_index(FaultCode fault_code);
    };

}  // namespace cotsbotics::coordinator
