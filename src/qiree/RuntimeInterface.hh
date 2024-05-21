//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/RuntimeInterface.hh
//---------------------------------------------------------------------------//
#pragma once

#include "Types.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Interface class for the \c rt (runtime) namespace.
 *
 * These four runtime functions must be implemented by all backends and are the
 * only four allowable by a "base profile" program:
 * https://github.com/qir-alliance/qir-spec/blob/main/specification/under_development/profiles/Base_Profile.md#runtime-functions
 * \verbatim
void @__quantum__rt__initialize(i8*)
void @__quantum__rt__array_record_output(i64, i8*)
void @__quantum__rt__result_record_output(%Result*, i8*)
void @__quantum__rt__tuple_record_output(i64, i8*)
 * \endverbatim
 *
 * Typical usage:
 * \verbatim
array_record_output(i64 3, i8* null);
result_record_output(%Result* null, i8* null)
result_record_output(%Result* inttoptr (i64 1 to %Result*), i8* null)
result_record_output(%Result* inttoptr (i64 2 to %Result*), i8* null)
 * \endverbatim
 */

class RuntimeInterface
{
  public:
    //! Initialize the execution environment, resetting qubits
    virtual void initialize(OptionalCString env) = 0;

    //! Mark the following N results as being part of an array named tag
    virtual void array_record_output(size_type, OptionalCString tag) = 0;

    //! Mark the following N results as being part of a tuple named tag
    virtual void tuple_record_output(size_type, OptionalCString tag) = 0;

    //! Record one result into the program output
    virtual void result_record_output(Result result, OptionalCString tag) = 0;

    virtual ~RuntimeInterface() = default;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
