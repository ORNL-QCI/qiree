//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/QsimTupleRuntime.cc
//---------------------------------------------------------------------------//
#include "QsimTupleRuntime.hh"

#include "qiree/Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits.
 */
void QsimTupleRuntime::initialize(OptionalCString env)
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
void QsimTupleRuntime::array_record_output(size_type s, OptionalCString tag)
{
    execute_if_needed();
    start_tracking(GroupingType::array, tag, s);
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and mark the following N results as being part of a tuple
 * named tag
 */
void QsimTupleRuntime::tuple_record_output(size_type s, OptionalCString tag)
{
    execute_if_needed();
    start_tracking(GroupingType::tuple, tag, s);
}

//---------------------------------------------------------------------------//
/*!
 * Execute circuit and report a single measurement result
 */
void QsimTupleRuntime::result_record_output(Result r, OptionalCString tag)
{
    execute_if_needed();
    Qubit q = sim_.result_to_qubit(r);
    push_result(q);
}

//---------------------------------------------------------------------------//
// PRIVATE FUNCTIONS
//---------------------------------------------------------------------------//

void QsimTupleRuntime::execute_if_needed()
{
    /*
    if (sim_.execute_if_needed() && print_accelbuf_)
    {
        sim_.print_accelbuf();
    }
    */
}

void QsimTupleRuntime::start_tracking(GroupingType type,
                                      std::string tag,
                                      size_type num_results)
{
    QIREE_EXPECT(!valid_);
    valid_ = true;
    type_ = type;
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

void QsimTupleRuntime::push_result(Qubit q)
{
    QIREE_EXPECT(valid_);
    QIREE_EXPECT(qubits_.size() < num_results_);
    qubits_.push_back(q);
    if (qubits_.size() == num_results_)
    {
        finish_tuple();
    }
}

void QsimTupleRuntime::print_header(size_type num_distinct)
{
    auto name = get_name();
    output_ << name << " " << tag_ << " length " << qubits_.size()
            << " distinct results " << num_distinct << std::endl;
}

void QsimTupleRuntime::finish_tuple()
{
    // auto counts = sim_.get_marginal_counts(qubits_);
    std::map<std::string, int> counts = {{"0", 0}, {"1", 0}};  // Placeholder
                                                               // for actual
                                                               // counts, TODO:
                                                               // replace with
                                                               // actual counts
    print_header(counts.size());
    auto name = get_name();
    for (auto& [bits, count] : counts)
    {
        output_ << name << " " << tag_ << " result " << bits << " count "
                << count << std::endl;
    }
    valid_ = false;
}
}  // namespace qiree
