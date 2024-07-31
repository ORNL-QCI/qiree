//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/XaccTupleRuntime.cc
//---------------------------------------------------------------------------//
#include "XaccTupleRuntime.hh"

#include "qiree/Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Construct an \c XaccTupleRuntime.
 *
 * The \c print_accelbuf argument determines whether the XACC \c
 * AcceleratorBuffer is dumped after execution.
 */
XaccTupleRuntime::XaccTupleRuntime(std::ostream& output,
                                   XaccQuantum& xacc,
                                   bool print_accelbuf)
    : output_(output)
    , xacc_(xacc)
    , print_accelbuf_(print_accelbuf)
    , valid_(false)
{
}

//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits.
 */
void XaccTupleRuntime::initialize(OptionalCString env)
{
    if (env)
    {
        output_ << "Argument to initialize: " << env << std::endl;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and mark the following N results as being part of an array
 * named tag.
 */
void XaccTupleRuntime::array_record_output(size_type s, OptionalCString tag)
{
    execute_if_needed();
    start_tracking(GroupingType::array, tag, s);
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and mark the following N results as being part of a tuple
 * named tag.
 */
void XaccTupleRuntime::tuple_record_output(size_type s, OptionalCString tag)
{
    execute_if_needed();
    start_tracking(GroupingType::tuple, tag, s);
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and report a single measurement result.
 */
void XaccTupleRuntime::result_record_output(Result r, OptionalCString tag)
{
    execute_if_needed();
    Qubit q = xacc_.result_to_qubit(r);
    push_result(q);
}

//---------------------------------------------------------------------------//
// PRIVATE FUNCTIONS
//---------------------------------------------------------------------------//

void XaccTupleRuntime::execute_if_needed()
{
    if (xacc_.execute_if_needed() && print_accelbuf_)
    {
        xacc_.print_accelbuf();
    }
}

void XaccTupleRuntime::start_tracking(GroupingType type,
                                      std::string tag,
                                      size_type num_results)
{
    QIREE_EXPECT(!valid_);
    valid_ = true;
    type_ = type_;
    tag_ = tag;
    num_results_ = num_results;
    qubits_.clear();

    if (!num_results_)
    {
        // Edge case
        print_header(0);
        valid_ = false;
    }
}

void XaccTupleRuntime::push_result(Qubit q)
{
    QIREE_EXPECT(valid_);
    QIREE_EXPECT(qubits_.size() < num_results_);
    qubits_.push_back(q);
    if (qubits_.size() == num_results_)
    {
        finish_tuple();
    }
}

void XaccTupleRuntime::print_header(size_type num_distinct)
{
    auto name = this->grouping_name();
    output_ << name << " " << tag_ << " length " << qubits_.size()
            << " distinct results " << num_distinct << std::endl;
}

void XaccTupleRuntime::finish_tuple()
{
    auto counts = xacc_.get_marginal_counts(qubits_);
    print_header(counts.size());
    auto name = this->grouping_name();
    for (auto& [bits, count] : counts)
    {
        output_ << name << " " << tag_ << " result " << bits << " count "
                << count << std::endl;
    }
    valid_ = false;
}

//---------------------------------------------------------------------------//
}  // namespace qiree
