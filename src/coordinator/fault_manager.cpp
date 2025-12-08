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

    void FaultManager::tick()
    {
        
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
        FaultConfig const& config = FaultConfiguration::get_fault_config(fault_code);
        // If this fault wasn't previously active, it's a new occurrence
        if (!fault.is_active)
        {
            fault.maturity_count++;
            if (fault.maturity_count >= config.maturity_count)
            {
                fault.total_occurrence_count++;
                fault.recovery_count = config.recovery_count;
                fault.maturity_count = 0; // Reset maturity count after becoming active
                fault.is_active = true;
            }
        }
        else
        {
            if (fault.recovery_count > 0)
                fault.recovery_count--;
            else
                fault.is_active = false;
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
        fault.maturity_count = 0;  // Reset maturity on recovery
        fault.recovery_count = 0;  // Reset recovery count
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

    uint8_t FaultManager::get_fault_maturity_count(FaultCode fault_code) const
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

        return _faults[index].maturity_count;
    }

    uint8_t FaultManager::get_fault_recovery_count(FaultCode fault_code) const
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

        return _faults[index].recovery_count;
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

        return _faults[index].total_occurrence_count;
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
            fault.maturity_count = 0;
            fault.recovery_count = 0;
            fault.total_occurrence_count = 0;
        }
    }

    uint8_t FaultManager::fault_code_to_index(FaultCode fault_code) const
    {
        return static_cast<uint8_t>(fault_code);
    }

}  // namespace cotsbotics::coordinator
