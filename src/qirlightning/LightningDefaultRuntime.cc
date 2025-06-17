//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirlightning/LightningDefaultRuntime.cc
//---------------------------------------------------------------------------//
#include "LightningDefaultRuntime.hh"

#include <iostream>

#include "qiree/Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits.
 */

void LightningDefaultRuntime::initialize(OptionalCString env)
{
    if (env)
    {
        output_ << "Argument to initialize: " << env << std::endl;
    }
}

}  // namespace qiree
