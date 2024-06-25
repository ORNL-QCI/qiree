//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/QuantumTestImpl.cc
//---------------------------------------------------------------------------//
#include "QuantumTestImpl.hh"

#include <gtest/gtest.h>

#include "Stream.hh"
#include "qiree/Assert.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//
/*!
 * Construct with pointer to modifiable test result.
 */
QuantumTestImpl::QuantumTestImpl(TestResult* result) : tr_{result}
{
    QIREE_EXPECT(tr_);
    tr_->commands << '\n';
}

//---------------------------------------------------------------------------//
/*!
 * Prepare to build a quantum circuit for an entry point.
 */
void QuantumTestImpl::set_up(EntryPointAttrs const& attrs)
{
    num_qubits_ = attrs.required_num_qubits;
    num_results_ = attrs.required_num_results;
    tr_->commands << "set_up(q=" << num_qubits_ << ", r=" << num_results_
                  << ")\n";
}

//---------------------------------------------------------------------------//
/*!
 * Complete an execution.
 */
void QuantumTestImpl::tear_down()
{
    tr_->commands << "tear_down\n";
}

//---------------------------------------------------------------------------//
/*!
 * Measure the qubit and store in the result.
 */
void QuantumTestImpl::mz(Qubit q, Result r)
{
    tr_->commands << "mz(" << q << "," << r << ")\n";
}

//---------------------------------------------------------------------------//
/*!
 * Read the value of a result.
 */
QState QuantumTestImpl::read_result(Result r)
{
    EXPECT_LT(r.value, this->num_results_);
    tr_->commands << "read_result(" << r << ")\n";
    return QState::zero;
}

//---------------------------------------------------------------------------//
/*!
 * Apply the H gate to the given qubit.
 */
void QuantumTestImpl::h(Qubit q)
{
    EXPECT_LT(q.value, this->num_qubits_);
    tr_->commands << "h(" << q << ")\n";
}

//---------------------------------------------------------------------------//
/*!
 * Apply the CNOT gate to the given qubits.
 */
void QuantumTestImpl::cnot(Qubit q1, Qubit q2)
{
    tr_->commands << "cnot(" << q1 << ", " << q2 << ")\n";
}

//---------------------------------------------------------------------------//
Result QuantumTestImpl::m(Qubit)
{
    tr_->commands << "TODO: m.body\n";
    return {};
}
Result QuantumTestImpl::measure(Array, Array)
{
    tr_->commands << "TODO: measure.body\n";
    return {};
}
Result QuantumTestImpl::mresetz(Qubit)
{
    tr_->commands << "TODO: mresetz.body\n";
    return {};
}
void QuantumTestImpl::ccx(Qubit, Qubit)
{
    tr_->commands << "TODO: ccx.body\n";
}
void QuantumTestImpl::cx(Qubit, Qubit)
{
    tr_->commands << "TODO: cx.body\n";
}
void QuantumTestImpl::cy(Qubit, Qubit)
{
    tr_->commands << "TODO: cy.body\n";
}
void QuantumTestImpl::cz(Qubit, Qubit)
{
    tr_->commands << "TODO: cz.body\n";
}
void QuantumTestImpl::exp_adj(Array, double, Array)
{
    tr_->commands << "TODO: exp.adj\n";
}
void QuantumTestImpl::exp(Array, double, Array)
{
    tr_->commands << "TODO: exp.body\n";
}
void QuantumTestImpl::exp(Array, Tuple)
{
    tr_->commands << "TODO: exp.ctl\n";
}
void QuantumTestImpl::exp_adj(Array, Tuple)
{
    tr_->commands << "TODO: exp.ctladj\n";
}
void QuantumTestImpl::h(Array, Qubit)
{
    tr_->commands << "TODO: h.ctl\n";
}
void QuantumTestImpl::r_adj(Pauli, double, Qubit)
{
    tr_->commands << "TODO: r.adj\n";
}
void QuantumTestImpl::r(Pauli, double, Qubit)
{
    tr_->commands << "TODO: r.body\n";
}
void QuantumTestImpl::r(Array, Tuple)
{
    tr_->commands << "TODO: r.ctl\n";
}
void QuantumTestImpl::r_adj(Array, Tuple)
{
    tr_->commands << "TODO: r.ctladj\n";
}
void QuantumTestImpl::reset(Qubit)
{
    tr_->commands << "TODO: reset.body\n";
}
void QuantumTestImpl::rx(double r, Qubit q)
{
    tr_->commands << "rx(" << r << ", " << q << ")\n";
}
void QuantumTestImpl::rx(Array, Tuple)
{
    tr_->commands << "TODO: rx.ctl\n";
}
void QuantumTestImpl::rxx(double, Qubit, Qubit)
{
    tr_->commands << "TODO: rxx.body\n";
}
void QuantumTestImpl::ry(double r, Qubit q)
{
    tr_->commands << "ry(" << r << ", " << q << ")\n";
}
void QuantumTestImpl::ry(Array, Tuple)
{
    tr_->commands << "TODO: ry.ctl\n";
}
void QuantumTestImpl::ryy(double, Qubit, Qubit)
{
    tr_->commands << "TODO: ryy.body\n";
}
void QuantumTestImpl::rz(double r, Qubit q)
{
    tr_->commands << "rz(" << r << ", " << q << ")\n";
}
void QuantumTestImpl::rz(Array, Tuple)
{
    tr_->commands << "TODO: rz.ctl\n";
}
void QuantumTestImpl::rzz(double, Qubit, Qubit)
{
    tr_->commands << "TODO: rzz.body\n";
}
void QuantumTestImpl::s_adj(Qubit)
{
    tr_->commands << "TODO: s.adj\n";
}
void QuantumTestImpl::s(Qubit q)
{
    tr_->commands << "s(" << q << ")\n";
}
void QuantumTestImpl::s(Array, Qubit)
{
    tr_->commands << "TODO: s.ctl\n";
}
void QuantumTestImpl::s_adj(Array, Qubit)
{
    tr_->commands << "TODO: s.ctladj\n";
}
void QuantumTestImpl::swap(Qubit, Qubit)
{
    tr_->commands << "TODO: swap.body\n";
}
void QuantumTestImpl::t_adj(Qubit)
{
    tr_->commands << "TODO: t.adj\n";
}
void QuantumTestImpl::t(Qubit)
{
    tr_->commands << "TODO: t.body\n";
}
void QuantumTestImpl::t(Array, Qubit)
{
    tr_->commands << "TODO: t.ctl\n";
}
void QuantumTestImpl::t_adj(Array, Qubit)
{
    tr_->commands << "TODO: t.ctladj\n";
}
void QuantumTestImpl::x(Qubit)
{
    tr_->commands << "TODO: x.body\n";
}
void QuantumTestImpl::x(Array, Qubit)
{
    tr_->commands << "TODO: x.ctl\n";
}
void QuantumTestImpl::y(Qubit)
{
    tr_->commands << "TODO: y.body\n";
}
void QuantumTestImpl::y(Array, Qubit)
{
    tr_->commands << "TODO: y.ctl\n";
}
void QuantumTestImpl::z(Qubit)
{
    tr_->commands << "TODO: z.body\n";
}
void QuantumTestImpl::z(Array, Qubit)
{
    tr_->commands << "TODO: z.ctl\n";
}

//---------------------------------------------------------------------------//
/*!
 * Construct with pointer to modifiable test result.
 */
ResultTestImpl::ResultTestImpl(TestResult* tr) : tr_{tr}
{
    QIREE_EXPECT(tr_);
}

//---------------------------------------------------------------------------//
/*!
 * Initialize the execution environment, resetting qubits
 */
void ResultTestImpl::initialize(OptionalCString env)
{
    tr_->commands << "initialize(";
    if (env)
    {
        tr_->commands << env;
    }
    tr_->commands << ")\n";
}

//---------------------------------------------------------------------------//
/*!
 * Prepare to store N results.
 */
void ResultTestImpl::array_record_output(size_type s, OptionalCString tag)
{
    ASSERT_GT(s, 0);
    tr_->commands << "array_record_output(" << s;
    if (tag)
    {
        tr_->commands << ", " << tag;
    }
    tr_->commands << ")\n";
}

//---------------------------------------------------------------------------//
/*!
 * Mark the start of an array and its size.
 */
void ResultTestImpl::result_record_output(Result result, OptionalCString tag)
{
    tr_->commands << "result_record_output(" << result;
    if (tag)
    {
        tr_->commands << ", " << tag;
    }
    tr_->commands << ")\n";
}

//---------------------------------------------------------------------------//
/*!
 * Mark the start of an array and its size.
 */
void ResultTestImpl::tuple_record_output(size_type s, OptionalCString tag)
{
    tr_->commands << "tuple_record_output(" << s;
    if (tag)
    {
        tr_->commands << ", " << tag;
    }
    tr_->commands << ")\n";
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
