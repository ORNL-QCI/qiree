//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/SingleResultRuntime.cc
//---------------------------------------------------------------------------//
#include "SingleResultRuntime.hh"

#include "QuantumInterface.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
//! Mark the following N results as being part of an array named tag
void SingleResultRuntime::array_record_output(size_type size,
                                              OptionalCString tag)
{
    result_ = RecordedResult(size, tag);
}

//! Mark the following N results as being part of a tuple named tag
void SingleResultRuntime::tuple_record_output(size_type size,
                                              OptionalCString tag)
{
    result_ = RecordedResult(size, tag);
}

//! Save one result
void SingleResultRuntime::result_record_output(Result result,
                                               OptionalCString tag)
{
    result_.push_back(sim_->read_result(result), tag);
}

//---------------------------------------------------------------------------//
}  // namespace qiree
