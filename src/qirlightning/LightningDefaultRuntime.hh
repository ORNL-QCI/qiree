//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirlightning/LightningDefaultRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "LightningQuantum.hh"
#include "qiree/RecordedResult.hh"

namespace qiree
{

/*!
 * Print per-qubit measurement statistics.
 *
 * Example for three qubits:
 * \code
 * Measurement output:
 * -------------------
 * Number of shots: 1024
 * Number of qubits: 3
 * q0 {0: 542, 1: 482}
 * q1 {0: 521, 1: 503}
 * q2 {0: 0, 1: 1024}
 *
 * \endcode
 */

class LightningDefaultRuntime final : virtual public RuntimeInterface
{
  public:
    /*!
     * Construct \c LightningDefaultRuntime.
     */
    LightningDefaultRuntime(std::ostream& output, LightningQuantum& sim)
        : output_(output), sim_(sim)
    {
    }

    //!@{
    //! \name Runtime interface

    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) override;

    //! Mark the following N results as being part of an array named tag
    void array_record_output(size_type size, OptionalCString tag) final
    {
        result_ = RecordedResult(size, tag);
    }

    //! Mark the following N results as being part of a tuple named tag
    void tuple_record_output(size_type size, OptionalCString tag) final
    {
        result_ = RecordedResult(size, tag);
    }

    //! Save one result
    void result_record_output(Result result, OptionalCString tag) final
    {
        result_.push_back(sim_.get_result(result), tag);
    }
    //!@}

    RecordedResult const& result() const { return result_; }


  private:
    std::ostream& output_;
    LightningQuantum& sim_;
    RecordedResult result_;
};

}  // namespace qiree
