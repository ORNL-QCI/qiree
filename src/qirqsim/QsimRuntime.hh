//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/QsimRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "qiree/SingleResultRuntime.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
class QsimQuantum;

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

class QsimRuntime final : virtual public SingleResultRuntime
{
  public:
    // Construct with quantum reference to access classical registers
    QsimRuntime(std::ostream& output, QsimQuantum const& sim);

    //!@{
    //! \name Runtime interface

    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) override;

    //!@}

  private:
    std::ostream& output_;
};

}  // namespace qiree
