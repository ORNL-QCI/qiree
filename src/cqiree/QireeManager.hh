//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/QireeManager.hh
//---------------------------------------------------------------------------//
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

namespace qiree
{
class Executor;
class Module;
class QuantumInterface;
class SingleResultRuntime;
class ResultDistribution;

//---------------------------------------------------------------------------//
/*!
 * Manage C execution sequence.
 *
 * The result size is min(2^classical registers, num_shots).
 */
class QireeManager
{
  public:
    enum class ReturnCode
    {
        //! The call succeeded
        success = 0,
        //! The function is not ready to be called
        not_ready,
        //! Invalid input
        invalid_input,
        //! Loading a module/result failed
        fail_load,
        //! Execution failed
        fail_execute,
    };
    using ResultRecord = std::array<std::uint64_t, 2>;

  public:
    QireeManager() throw();
    ~QireeManager() throw();

    ReturnCode load_module(std::string_view data_contents) throw();
    ReturnCode load_module(std::string filename) throw();
    ReturnCode num_quantum_reg(int& result) const throw();
    ReturnCode num_classical_reg(int& result) const throw();
    ReturnCode setup_executor(std::string_view backend,
                              std::string_view config_json = {}) throw();

    ReturnCode max_result_items(int num_shots, std::size_t& result) const
        throw();

    ReturnCode execute(int num_shots) throw();

    ReturnCode
    save_result_items(ResultRecord* encoded, std::size_t size) throw();

  private:
    std::unique_ptr<Module> module_;
    std::unique_ptr<Executor> execute_;
    std::shared_ptr<QuantumInterface> quantum_;
    std::shared_ptr<SingleResultRuntime> runtime_;
    std::unique_ptr<ResultDistribution> result_;
};

}  // namespace qiree
