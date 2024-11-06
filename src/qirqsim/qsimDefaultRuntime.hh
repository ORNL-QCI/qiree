//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/qsimDefaultRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "qsimQuantum.hh"

namespace qiree
{

/*!
 * Print per-qubit measurement statistics. 
 *
 * Example for three qubits:
 * \code
 * q0 |0> freq: 509
 * q0 |1> freq: 515
 * q1 |0> freq: 509
 * q1 |1> freq: 515
 * q2 |1> freq: 1024
 * \endcode
 */

class qsimDefaultRuntime final : virtual public RuntimeInterface
{
  public:
    /*!
     * Construct \c qsimDefaultRuntime.
     */
    qsimDefaultRuntime(std::ostream& output,
                       qsimQuantum& sim
                       )
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
    qsimQuantum& sim_;
};

}  // namespace qiree
