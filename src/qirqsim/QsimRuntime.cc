//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/QsimRuntime.cc
//---------------------------------------------------------------------------//
#include "QsimRuntime.hh"

#include <iostream>

#include "QsimQuantum.hh"
#include "qiree/Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Construct with quantum reference to access classical registers.
 */
QsimRuntime::QsimRuntime(std::ostream& output, QsimQuantum const& sim)
    : SingleResultRuntime{sim}, output_(output)
{
}

//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits.
 */
void QsimRuntime::initialize(OptionalCString env)
{
    if (env)
    {
        output_ << "Argument to initialize: " << env << std::endl;
    }
}

}  // namespace qiree
