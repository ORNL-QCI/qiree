//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/SingleResultRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "RecordedResult.hh"
#include "RuntimeInterface.hh"
#include "qiree/Assert.hh"

namespace qiree
{
class QuantumInterface;
//---------------------------------------------------------------------------//
/*!
 * Interface class for recording a single shot per execution.
 */
class SingleResultRuntime : public RuntimeInterface
{
  public:
    // Mark the following N results as being part of an array named tag
    void array_record_output(size_type size, OptionalCString tag) final;

    // Mark the following N results as being part of a tuple named tag
    void tuple_record_output(size_type size, OptionalCString tag) final;

    // Save one result
    void result_record_output(Result result, OptionalCString tag) final;

    //! Access the saved results
    RecordedResult const& result() const { return result_; }

  protected:
    // This class should only be constructed from a daughter
    explicit SingleResultRuntime(QuantumInterface const& sim) : sim_{&sim} {}

  private:
    QuantumInterface const* sim_;
    RecordedResult result_;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
