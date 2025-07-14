//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/QireeManager.cc
//---------------------------------------------------------------------------//
#include "QireeManager.hh"

#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "qiree_config.h"

#include "qiree/Assert.hh"
#include "qiree/Executor.hh"
#include "qiree/Module.hh"
#include "qiree/QuantumInterface.hh"
#include "qiree/ResultDistribution.hh"
#include "qiree/SingleResultRuntime.hh"
#include "qirqsim/QsimQuantum.hh"
#include "qirqsim/QsimRuntime.hh"

#define CQIREE_FAIL(CODE, MESSAGE)                         \
    do                                                     \
    {                                                      \
        std::cerr << "qiree failure: " << MESSAGE << '\n'; \
        return ReturnCode::CODE;                           \
    } while (0)

namespace qiree
{
//---------------------------------------------------------------------------//
QireeManager::QireeManager() throw() = default;
QireeManager::~QireeManager() throw() = default;

//---------------------------------------------------------------------------//
QireeManager::ReturnCode
QireeManager::load_module(std::string_view data_contents) throw()
{
    try
    {
        // Convert string_view to string for Module constructor
        std::string content{data_contents};
        module_ = std::make_unique<Module>(content);
    }
    catch (std::exception const& e)
    {
        CQIREE_FAIL(fail_load, e.what());
    }
    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::load_module(std::string filename) throw()
{
    try
    {
        module_ = std::make_unique<Module>(filename);
        QIREE_ENSURE(*module_);
    }
    catch (std::exception const& e)
    {
        std::cerr << "qiree failure: " << e.what() << '\n';
        CQIREE_FAIL(fail_load, e.what());
    }

    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::num_quantum_reg(int& result) const
    throw()
{
    if (!module_)
    {
        CQIREE_FAIL(not_ready, "cannot query module before module load");
    }
    if (execute_)
    {
        CQIREE_FAIL(not_ready, "cannot query module after creating executor");
    }

    auto attrs = module_->load_entry_point_attrs();
    result = attrs.required_num_qubits;
    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::num_classical_reg(int& result) const
    throw()
{
    if (!module_)
    {
        CQIREE_FAIL(not_ready, "cannot query module before module load");
    }
    if (execute_)
    {
        CQIREE_FAIL(not_ready, "cannot query module after creating executor");
    }

    auto attrs = module_->load_entry_point_attrs();
    result = attrs.required_num_results;
    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode
QireeManager::max_result_items(int num_shots, std::size_t& result) const
    throw()
{
    if (!module_)
    {
        CQIREE_FAIL(not_ready,
                    "cannot determine result size before module load");
    }

    if (num_shots <= 0)
    {
        CQIREE_FAIL(invalid_input, "num_shots was nonpositive");
    }

    // Maximum result
    auto attrs = module_->load_entry_point_attrs();
    auto num_registers = attrs.required_num_results;
    result = std::min<std::size_t>(1 << num_registers, num_shots);

    // Add one for the "count" record at the beginning
    result++;

    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode
QireeManager::setup_executor(std::string_view backend,
                             std::string_view config_json) throw()
{
    if (!module_)
    {
        CQIREE_FAIL(not_ready, "cannot create executor before module load");
    }
    if (execute_)
    {
        CQIREE_FAIL(not_ready, "cannot create executor again");
    }

    try
    {
        if (!config_json.empty())
        {
            QIREE_NOT_IMPLEMENTED("CQiree JSON configuration input");
        }

        if (backend == "qsim")
        {
#if QIREE_USE_QSIM
            // Create runtime interface: give runtime a pointer to quantum
            // (lifetime of the reference is guaranteed by our shared pointer
            // copy)
            constexpr unsigned long int seed = 0;
            auto quantum = std::make_shared<QsimQuantum>(std::cout, seed);
            runtime_ = std::make_shared<QsimRuntime>(std::cout, *quantum);
            quantum_ = std::move(quantum);
#else
            QIREE_NOT_CONFIGURED("QSim");
#endif
        }
        else if (backend == "xacc")
        {
#if QIREE_USE_XACC
            QIREE_NOT_IMPLEMENTED("XACC backend for CQiree");
#else
            QIREE_NOT_CONFIGURED("XACC");
#endif
        }
        else
        {
            QIREE_VALIDATE(false,
                           << "unknown backend name '" << backend << "'");
        }
    }
    catch (std::exception const& e)
    {
        CQIREE_FAIL(fail_load,
                    "error while creating quantum runtimes: " << e.what());
    }

    try
    {
        // Create executor with the module, quantum and runtime interfaces
        QIREE_ASSERT(module_ && *module_);
        execute_ = std::make_unique<Executor>(std::move(*module_));
    }
    catch (std::exception const& e)
    {
        CQIREE_FAIL(fail_load, "error while creating executor: " << e.what());
    }

    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::execute(int num_shots) throw()
{
    if (!execute_)
    {
        CQIREE_FAIL(not_ready, "setup_executor was not created");
    }

    if (num_shots <= 0)
    {
        CQIREE_FAIL(invalid_input, "num_shots was nonpositive");
    }

    try
    {
        QIREE_ASSERT(runtime_ && quantum_);
        result_ = std::make_unique<ResultDistribution>();

        for (auto i = 0; i < num_shots; ++i)
        {
            (*execute_)(*quantum_, *runtime_);
            result_->accumulate(runtime_->result());
        }
    }
    catch (std::exception const& e)
    {
        CQIREE_FAIL(fail_execute, e.what());
    }
    return ReturnCode::success;
}
//---------------------------------------------------------------------------//
QireeManager::ReturnCode
QireeManager::save_result_items(ResultRecord* encoded,
                                std::size_t max_count) throw()
{
    if (!result_)
    {
        CQIREE_FAIL(not_ready, "execute has not been called");
    }

    if (max_count < result_->size() + 1)
    {
        CQIREE_FAIL(fail_load,
                    "insufficient capacity " << max_count
                                             << " for result items: need "
                                             << result_->size() + 1);
    }

    try
    {
        // Save number of records
        *encoded++ = {0, result_->size()};
        for (auto&& [bitstring, count] : *result_)
        {
            std::uint64_t bitint{0};
            encode_bit_string(bitstring, bitint);
            *encoded++ = {bitint, count};
        }
    }
    catch (std::exception const& e)
    {
        CQIREE_FAIL(fail_execute, e.what());
    }
    return ReturnCode::success;
}
//---------------------------------------------------------------------------//
}  // namespace qiree