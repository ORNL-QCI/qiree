//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirlightning/LightningRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "qiree/SingleResultRuntime.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
class LightningQuantum;

//---------------------------------------------------------------------------//

class LightningRuntime final : virtual public SingleResultRuntime
{
  public:
    // Construct with quantum reference to access classical registers
    LightningRuntime(std::ostream& output, LightningQuantum const& sim);

    //!@{
    //! \name Runtime interface

    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) override;

    //!@}

  private:
    std::ostream& output_;
};

}  // namespace qiree
