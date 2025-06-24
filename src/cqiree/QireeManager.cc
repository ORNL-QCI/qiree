//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/QireeManager.cc
//---------------------------------------------------------------------------//
#include "QireeManager.hh"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace qiree
{
//---------------------------------------------------------------------------//
QireeManager::ReturnCode
QireeManager::load_module(std::string_view data_contents) throw()
{
    try
    {
        // Convert string_view to string for Module constructor
        std::string content{data_contents};
        module_ = std::make_unique<Module>(content);
        return ReturnCode::success;
    }
    catch (std::exception const& e)
    {
        std::cerr << "qiree failure: " << e.what() << '\n';
        return ReturnCode::fail_load;
    }
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::load_module(std::string filename)
{
    try
    {
        module_ = std::make_unique<Module>(filename);

        if (!*module_)
        {
            return ReturnCode::fail_load;
        }
        return ReturnCode::success;
    }
    catch (std::exception const& e)
    {
        std::cerr << "qiree failure: " << e.what() << '\n';
        return ReturnCode::fail_load;
    }
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::num_quantum_reg(int& result) const
{
    if (!module_)
    {
        result = 0;
        return ReturnCode::fail_load;
    }

    auto attrs = module_->load_entry_point_attrs();
    result = attrs.required_qubits;
    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::num_classical_reg(int& result) const
{
    if (!module_)
    {
        result = 0;
        return ReturnCode::fail_load;
    }

    auto attrs = module_->load_entry_point_attrs();
    result = attrs.required_results;
    return ReturnCode::success;
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode
QireeManager::setup_executor(std::string_view backend,
                             std::string_view config_json)
{
    if (!module_)
    {
        return ReturnCode::fail_load;
    }

    try
    {
        // Create the quantum backend based on the backend name
        quantum_
            = create_backend(std::string{backend}, std::string{config_json});
        if (!quantum_)
        {
            return ReturnCode::fail_load;
        }

        // Create runtime interface
        runtime_ = std::make_shared<RuntimeInterface>();

        // Create executor with the module, quantum and runtime interfaces
        executor_ = std::make_unique<Executor>(
            std::move(*module_), quantum_, runtime_);
        return ReturnCode::success;
    }
    catch (std::exception const& e)
    {
        std::cerr << "qiree failure: " << e.what() << '\n';
        return ReturnCode::fail_load;
    }
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::execute(int num_shots)
{
    if (!executor_)
    {
        return ReturnCode::fail_load;
    }

    try
    {
        executor_->run(static_cast<size_t>(num_shots));
        return ReturnCode::success;
    }
    catch (std::exception const& e)
    {
        std::cerr << "qiree failure: " << e.what() << '\n';
        return ReturnCode::fail_load;
    }
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode QireeManager::num_results(int& count) const
{
    if (!quantum_)
    {
        count = 0;
        return ReturnCode::fail_load;
    }

    try
    {
        // Get the number of results from the quantum interface
        count = static_cast<int>(quantum_->results().size());
        return ReturnCode::success;
    }
    catch (std::exception const& e)
    {
        std::cerr << "qiree failure: " << e.what() << '\n';
        count = 0;
        return ReturnCode::fail_load;
    }
}

//---------------------------------------------------------------------------//
QireeManager::ReturnCode
QireeManager::get_result(int index, std::string_view key, int* count) const
{
    if (!quantum_ || !count)
    {
        return ReturnCode::fail_load;
    }

    try
    {
        // Get the results from the quantum interface
        auto results = quantum_->results();
        if (index < 0 || static_cast<size_t>(index) >= results.size())
        {
            return ReturnCode::fail_load;
        }

        // Get the result count for the specified key
        std::string key_str{key};
        auto it = results[index].find(key_str);
        if (it == results[index].end())
        {
            *count = 0;
            return ReturnCode::success;
        }

        *count = static_cast<int>(it->second);
    }
    catch (std::exception const& e)
    {
        std::cerr << "qiree failure: " << e.what() << '\n';
        return ReturnCode::fail_load;
    }
}
//---------------------------------------------------------------------------//
}  // namespace qiree