//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/QsimDefaultRuntime.cc
//---------------------------------------------------------------------------//
#include "QsimDefaultRuntime.hh"

#include <iostream>

#include "qiree/Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits.
 */

void QsimDefaultRuntime::initialize(OptionalCString env)
{
    if (env)
    {
        output_ << "Argument to initialize: " << env << std::endl;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and mark the following N results as being part of an array
 * named tag
 */

void QsimDefaultRuntime::array_record_output(size_type s, OptionalCString tag)
{
    // this->execute_if_needed();
    // output_ << "array " << (tag ? tag : "<null>") << " length " << s
    //         << std::endl;
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and mark the following N results as being part of a tuple
 * named tag
 */

void QsimDefaultRuntime::tuple_record_output(size_type s, OptionalCString tag)
{
    // this->execute_if_needed();
    // output_ << "tuple " << (tag ? tag : "<null>") << " length " << s
    //         << std::endl;
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and report a single measurement result
 */
void QsimDefaultRuntime::result_record_output(Result r, OptionalCString tag)
{
    // Access values through the getter
    // This prints results every time result_record_output is called
    // Can comment out if only want to see final results

    if (auto value = sim_.manager.getBufferValue("q" + std::to_string(r.value));
        value.has_value())
    {
        std::cout << "q" << std::to_string(r.value) << " : " << value.value()
                  << "\n";
    }
}

}  // namespace qiree
