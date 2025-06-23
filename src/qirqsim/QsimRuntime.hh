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

//---------------------------------------------------------------------------//

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
