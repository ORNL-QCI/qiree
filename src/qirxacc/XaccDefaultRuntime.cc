//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/XaccDefaultRuntime.cc
//---------------------------------------------------------------------------//
#include "XaccDefaultRuntime.hh"

#include "qiree/Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits.
 */
void XaccDefaultRuntime::initialize(OptionalCString env)
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
void XaccDefaultRuntime::array_record_output(size_type s, OptionalCString tag)
{
    this->execute_if_needed();
    output_ << "array " << (tag ? tag : "<null>") << " length " << s
            << std::endl;
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and mark the following N results as being part of a tuple
 * named tag.
 */
void XaccDefaultRuntime::tuple_record_output(size_type s, OptionalCString tag)
{
    this->execute_if_needed();
    output_ << "tuple " << (tag ? tag : "<null>") << " length " << s
            << std::endl;
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and report a single measurement result
 */
void XaccDefaultRuntime::result_record_output(Result r, OptionalCString tag)
{
    this->execute_if_needed();
    Qubit q = xacc_.result_to_qubit(r);

    // Get a map of string ("0" and "1" ???) -> int
    auto counts = xacc_.get_marginal_counts({q});

    // Print the result
    output_ << "qubit " << q.value << " experiment " << (tag ? tag : "<null>")
            << ": {0: " << counts["0"] << ", 1: " << counts["1"] << '}'
            << std::endl;
}

//---------------------------------------------------------------------------//
// PRIVATE FUNCTIONS
//---------------------------------------------------------------------------//

void XaccDefaultRuntime::execute_if_needed()
{
    if (xacc_.execute_if_needed() && print_accelbuf_)
    {
        xacc_.print_accelbuf();
    }
}
}  // namespace qiree
