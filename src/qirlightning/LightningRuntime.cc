//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirlightning/LightningRuntime.cc
//---------------------------------------------------------------------------//
#include "LightningRuntime.hh"

#include <iostream>

#include "LightningQuantum.hh"
#include "qiree/Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Construct with quantum reference to access classical registers.
 */
LightningRuntime::LightningRuntime(std::ostream& output, LightningQuantum const& sim)
    : SingleResultRuntime{sim}, output_(output)
{
}

//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits.
 */

void LightningRuntime::initialize(OptionalCString env)
{
    if (env)
    {
        output_ << "Argument to initialize: " << env << std::endl;
    }
}

}  // namespace qiree
