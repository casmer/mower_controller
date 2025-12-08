// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once
#define ETL_NO_STL

#include <stdint.h>
#include <etl/array.h>

namespace cotsbotics::coordinator
{
    /// @brief Enum defining all possible faults in the system
    enum class FaultCode : uint8_t
    {
        NO_FAULT = 0,
        INTERLOC_MISMATCH_ERROR,
        SBUS_RECEIVER_ERROR,
        RADIO_LOSS_OF_SIGNAL,
        MAX_FAULT_CODE
    };

    /// @brief Represents the state and maturity of a single fault
    struct FaultState
    {
        /// @brief Whether this fault is currently active
        bool is_active{false};
        
        /// @brief number of ticks the fault has been active (maturity counter)
        uint32_t maturity_count{0};
        
        /// @brief number of ticks since the fault was last resolved
        uint32_t recovery_count{0};
        
        /// @brief Total count of how many times this fault has occurred
        uint16_t total_occurrence_count{0};
    };

    /// @brief Manages system faults, tracking maturity and recovery
    class FaultManager
    {
    public:
        FaultManager();
        ~FaultManager();

        /// @brief Initialize the fault manager
        void setup();

        /// @brief Periodic tick to update fault maturity and recovery counters
        /// @param delta_ms Time elapsed since last tick in milliseconds
        void tick();

        /// @brief Report a fault occurrence
        /// @param fault_code The fault to report
        void report_fault(FaultCode fault_code);

        /// @brief Clear/resolve a fault
        /// @param fault_code The fault to clear
        void clear_fault(FaultCode fault_code);

        /// @brief Get the maturity of a fault in milliseconds
        /// @param fault_code The fault to query
        /// @return Maturity time in milliseconds, 0 if fault is inactive
        uint8_t get_fault_maturity_count(FaultCode fault_code) const;

        /// @brief Get the recovery count of a fault in milliseconds
        /// @param fault_code The fault to query
        /// @return Recovery time in milliseconds since last resolution
        uint8_t get_fault_recovery_count(FaultCode fault_code) const;

        /// @brief Get the occurrence count of a fault
        /// @param fault_code The fault to query
        /// @return Number of times this fault has occurred
        uint16_t get_fault_occurrence_count(FaultCode fault_code) const;

        /// @brief Check if any fault is currently active
        /// @return true if any fault is active, false if system is healthy
        bool has_active_faults() const;

        /// @brief Get all fault states
        /// @return Reference to the fault state array
        const etl::array<FaultState, static_cast<uint8_t>(FaultCode::MAX_FAULT_CODE)>& get_all_faults() const;

        /// @brief Reset all fault history
        void reset_all_faults();

        /// @brief Check if a fault is currently active
        /// @param fault_code The fault to check
        /// @return true if the fault is active, false otherwise
        bool is_fault_active(FaultCode fault_code) const;

    private:
        /// @brief Array storing the state of all faults
        etl::array<FaultState, static_cast<uint8_t>(FaultCode::MAX_FAULT_CODE)> _faults;

        /// @brief Convert FaultCode to array index
        /// @param fault_code The fault code to convert
        /// @return Index into the _faults array
        uint8_t fault_code_to_index(FaultCode fault_code) const;
    };

}