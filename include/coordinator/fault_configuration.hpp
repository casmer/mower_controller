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
        constexpr FaultConfig(uint32_t maturity_ms, uint32_t recovery_ms)
            : maturity_threshold_ms(maturity_ms),
              recovery_threshold_ms(recovery_ms)
        {
        }
        /// @brief Time in milliseconds a fault must persist to be considered "mature"
        uint32_t maturity_threshold_ms{0};

        /// @brief Time in milliseconds required for recovery/resolution of a fault
        uint32_t recovery_threshold_ms{0};
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
            // NO_FAULT (0)
            FaultConfig(0, 0),
            
            // RADIO_LOSS_OF_SIGNAL (1) - Critical, should mature quickly and recover quickly
            FaultConfig(100, 500),
            
            // SBUS_RECEIVER_ERROR (2) - High priority communication fault
            FaultConfig(50, 1000),
            
            // MOTOR_DRIVER_FAULT_LEFT (3) - Critical drive fault
            FaultConfig(200, 2000),
            
            // MOTOR_DRIVER_FAULT_RIGHT (4) - Critical drive fault
            FaultConfig(200, 2000),
            
            // ADC_COMMUNICATION_ERROR (5) - Sensor fault
            FaultConfig(500, 3000),
            
            // DAC_COMMUNICATION_ERROR (6) - Output fault
            FaultConfig(500, 3000),
            
            // SEAT_SWITCH_FAULT (7) - Safety critical
            FaultConfig(100, 1000),
            
            // BRAKE_SYSTEM_FAULT (8) - Safety critical
            FaultConfig(150, 2000),
            
            // BLADE_MOTOR_FAULT (9) - Important but not critical
            FaultConfig(300, 2000),
            
            // BATTERY_LOW (10) - Warning level, slower maturity
            FaultConfig(5000, 10000),
            
            // WATCHDOG_TIMEOUT (11) - System level fault
            FaultConfig(10, 5000),  
            
            // INVALID_STATE_TRANSITION (12) - Logic fault
            FaultConfig(100, 2000),
            
            // SENSOR_OUT_OF_RANGE (13) - Sensor anomaly
            FaultConfig(200, 1000),
        }};

        /// @brief Convert FaultCode to array index
        static uint8_t fault_code_to_index(FaultCode fault_code);
    };

}  // namespace cotsbotics::coordinator
