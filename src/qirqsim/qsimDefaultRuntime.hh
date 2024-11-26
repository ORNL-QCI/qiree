//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/QsimDefaultRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "QsimQuantum.hh"

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

class QsimDefaultRuntime final : virtual public RuntimeInterface
{
  public:
    /*!
     * Construct \c QsimDefaultRuntime.
     */
    QsimDefaultRuntime(std::ostream& output, QsimQuantum& sim)
        : output_(output), sim_(sim)
    {
    }

    //!@{
    //! \name Runtime interface
    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) override;

    //! Mark the following N results as being part of an array named tag
    void array_record_output(size_type, OptionalCString tag) final;

    //! Mark the following N results as being part of a tuple named tag
    void tuple_record_output(size_type, OptionalCString) final;

    // Save one result
    void result_record_output(Result result, OptionalCString tag) final;
    //!@}

  private:
    std::ostream& output_;
    QsimQuantum& sim_;
};

}  // namespace qiree
