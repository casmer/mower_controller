// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "coordinator/fault_manager.hpp"
#include "coordinator/fault_configuration.hpp"

namespace cotsbotics::coordinator
{
    FaultManager::FaultManager()
    {
        reset_all_faults();
    }

    FaultManager::~FaultManager()
    {
    }

    void FaultManager::setup()
    {
        reset_all_faults();
    }

    void FaultManager::tick(uint32_t delta_ms)
    {
        // Update maturity counters for active faults
        for (auto& fault : _faults)
        {
            if (fault.is_active)
            {
                fault.maturity_ms += delta_ms;
            }
            else if (fault.recovery_time_ms > 0 || fault.occurrence_count > 0)
            {
                // Track recovery time for resolved faults
                fault.recovery_time_ms += delta_ms;
            }
        }
    }

    void FaultManager::report_fault(FaultCode fault_code)
    {
        if (fault_code == FaultCode::NO_FAULT)
        {
            return;
        }

        uint8_t index = fault_code_to_index(fault_code);
        if (index >= _faults.size())
        {
            return;
        }

        FaultState& fault = _faults[index];

        // If this fault wasn't previously active, it's a new occurrence
        if (!fault.is_active)
        {
            fault.occurrence_count++;
            fault.recovery_time_ms = 0;  // Reset recovery timer on re-occurrence
        }

        fault.is_active = true;
    }

    void FaultManager::clear_fault(FaultCode fault_code)
    {
        if (fault_code == FaultCode::NO_FAULT)
        {
            return;
        }

        uint8_t index = fault_code_to_index(fault_code);
        if (index >= _faults.size())
        {
            return;
        }

        FaultState& fault = _faults[index];
        fault.is_active = false;
        fault.maturity_ms = 0;  // Reset maturity on recovery
    }

    bool FaultManager::is_fault_active(FaultCode fault_code) const
    {
        if (fault_code == FaultCode::NO_FAULT)
        {
            return false;
        }

        uint8_t index = fault_code_to_index(fault_code);
        if (index >= _faults.size())
        {
            return false;
        }

        return _faults[index].is_active;
    }

    uint32_t FaultManager::get_fault_maturity_ms(FaultCode fault_code) const
    {
        if (fault_code == FaultCode::NO_FAULT)
        {
            return 0;
        }

        uint8_t index = fault_code_to_index(fault_code);
        if (index >= _faults.size())
        {
            return 0;
        }

        return _faults[index].maturity_ms;
    }

    uint32_t FaultManager::get_fault_recovery_time_ms(FaultCode fault_code) const
    {
        if (fault_code == FaultCode::NO_FAULT)
        {
            return 0;
        }

        uint8_t index = fault_code_to_index(fault_code);
        if (index >= _faults.size())
        {
            return 0;
        }

        return _faults[index].recovery_time_ms;
    }

    uint16_t FaultManager::get_fault_occurrence_count(FaultCode fault_code) const
    {
        if (fault_code == FaultCode::NO_FAULT)
        {
            return 0;
        }

        uint8_t index = fault_code_to_index(fault_code);
        if (index >= _faults.size())
        {
            return 0;
        }

        return _faults[index].occurrence_count;
    }

    bool FaultManager::has_active_faults() const
    {
        for (const auto& fault : _faults)
        {
            if (fault.is_active)
            {
                return true;
            }
        }
        return false;
    }

    const etl::array<FaultState, static_cast<uint8_t>(FaultCode::MAX_FAULT_CODE)>& 
    FaultManager::get_all_faults() const
    {
        return _faults;
    }

    void FaultManager::reset_all_faults()
    {
        for (auto& fault : _faults)
        {
            fault.is_active = false;
            fault.maturity_ms = 0;
            fault.recovery_time_ms = 0;
            fault.occurrence_count = 0;
        }
    }

    uint8_t FaultManager::fault_code_to_index(FaultCode fault_code) const
    {
        return static_cast<uint8_t>(fault_code);
    }

    bool FaultManager::is_fault_mature(FaultCode fault_code) const
    {
        if (fault_code == FaultCode::NO_FAULT || !is_fault_active(fault_code))
        {
            return false;
        }

        const FaultConfig& config = FaultConfiguration::get_fault_config(fault_code);
        uint32_t maturity = get_fault_maturity_ms(fault_code);

        return maturity >= config.maturity_threshold_ms;
    }

    bool FaultManager::has_fault_recovered(FaultCode fault_code) const
    {
        if (fault_code == FaultCode::NO_FAULT || is_fault_active(fault_code))
        {
            return false;
        }

        const FaultConfig& config = FaultConfiguration::get_fault_config(fault_code);
        uint32_t recovery = get_fault_recovery_time_ms(fault_code);

        return recovery >= config.recovery_threshold_ms;
    }

}  // namespace cotsbotics::coordinator
