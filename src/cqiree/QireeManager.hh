//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/QireeManager.hh
//---------------------------------------------------------------------------//
#pragma once

#include <memory>
#include <string_view>

#include "qiree/Executor.hh"
#include "qiree/Module.hh"
#include "qiree/QuantumInterface.hh"
#include "qiree/RuntimeInterface.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Manage C execution sequence.
 */
class QireeManager
{
  public:
    enum class ReturnCode
    {
        success = 0,
        fail_load,
        // ...
    };

  public:
    ReturnCode load_module(std::string_view data_contents);
    ReturnCode load_module(std::string filename);
    ReturnCode num_quantum_reg(int& result) const;
    ReturnCode num_classical_reg(int& result) const;
    ReturnCode setup_executor(std::string_view backend,
                              std::string_view config_json = {});

    ReturnCode execute(int num_shots);
    ReturnCode num_results(int& count) const;
    ReturnCode get_result(int index, std::string_view key, int* count) const;

  private:
    std::unique_ptr<Module> module_;
    std::unique_ptr<Executor> executor_;
    std::shared_ptr<QuantumInterface> quantum_;
    std::shared_ptr<RuntimeInterface> runtime_;
};

}  // namespace qiree