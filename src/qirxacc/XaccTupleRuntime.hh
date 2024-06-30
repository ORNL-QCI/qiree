//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/XaccTupleRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "qirxacc/XaccQuantum.hh"

namespace qiree
{
class XaccQuantum;

//---------------------------------------------------------------------------//
/*!
 * Print per-tuple (or per-array) measurement statistics.
 *
 * (Compare with \ref XaccDefaultRuntime.)
 *
 * Example:
 * \code
 * tuple ret length 2 distinct results 2
 * tuple ret result 00 count 512
 * tuple ret result 11 count 512
 * \endcode
 */
class XaccTupleRuntime final : virtual public RuntimeInterface
{
  public:
    // Construct with XACC quantum runtime and options
    XaccTupleRuntime(std::ostream& output,
                     XaccQuantum& xacc,
                     bool print_accelbuf);

    //!@{
    //! \name Runtime interface
    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) override;

    // Execute circuit and mark the following N results as being part of an
    // array named tag
    void array_record_output(size_type, OptionalCString tag) final;

    // Execute circuit and mark the following N results as being part of a
    // tuple named tag
    void tuple_record_output(size_type, OptionalCString) final;

    // Execute circuit and report a single measurement result
    void result_record_output(Result result, OptionalCString tag) final;
    //!@}

  private:
    enum class GroupingType
    {
        tuple,
        array,
    };

    std::ostream& output_;
    XaccQuantum& xacc_;
    bool const print_accelbuf_;
    bool valid_;
    GroupingType type_;
    std::string tag_;
    size_type num_results_;
    std::vector<Qubit> qubits_;

    void execute_if_needed();
    void
    start_tracking(GroupingType type, std::string tag, size_type num_results);
    void push_result(Qubit q);
    void print_header(size_type num_distinct);
    void finish_tuple();

    char const* const grouping_name() const
    {
        return type_ == GroupingType::tuple   ? "tuple"
               : type_ == GroupingType::array ? "array"
                                              : "grouping";
    }
};

//---------------------------------------------------------------------------//
}  // namespace qiree
