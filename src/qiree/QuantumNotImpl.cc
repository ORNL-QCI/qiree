//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/QuantumNotImpl.cc
//---------------------------------------------------------------------------//
#include "QuantumNotImpl.hh"

#include "Assert.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
Result QuantumNotImpl::m(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'm.body'");
}
Result QuantumNotImpl::measure(Array, Array)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'measure.body'");
}
Result QuantumNotImpl::mresetz(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'mresetz.body'");
}
void QuantumNotImpl::mz(Qubit, Result)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'mz.body'");
}
QState QuantumNotImpl::read_result(Result)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'read_result.body'");
}
void QuantumNotImpl::ccx(Qubit, Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'ccx.body'");
}
void QuantumNotImpl::cnot(Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'cnot.body'");
}
void QuantumNotImpl::cx(Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'cx.body'");
}
void QuantumNotImpl::cy(Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'cy.body'");
}
void QuantumNotImpl::cz(Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'cz.body'");
}
void QuantumNotImpl::exp_adj(Array, double, Array)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'exp.adj'");
}
void QuantumNotImpl::exp(Array, double, Array)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'exp.body'");
}
void QuantumNotImpl::exp(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'exp.ctl'");
}
void QuantumNotImpl::exp_adj(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'exp.ctladj'");
}
void QuantumNotImpl::h(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'h.body'");
}
void QuantumNotImpl::h(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'h.ctl'");
}
void QuantumNotImpl::r_adj(Pauli, double, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'r.adj'");
}
void QuantumNotImpl::r(Pauli, double, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'r.body'");
}
void QuantumNotImpl::r(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'r.ctl'");
}
void QuantumNotImpl::r_adj(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'r.ctladj'");
}
void QuantumNotImpl::reset(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'reset.body'");
}
void QuantumNotImpl::rx(double, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'rx.body'");
}
void QuantumNotImpl::rx(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'rx.ctl'");
}
void QuantumNotImpl::rxx(double, Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'rxx.body'");
}
void QuantumNotImpl::ry(double, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'ry.body'");
}
void QuantumNotImpl::ry(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'ry.ctl'");
}
void QuantumNotImpl::ryy(double, Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'ryy.body'");
}
void QuantumNotImpl::rz(double, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'rz.body'");
}
void QuantumNotImpl::rz(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'rz.ctl'");
}
void QuantumNotImpl::rzz(double, Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'rzz.body'");
}
void QuantumNotImpl::s_adj(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 's.adj'");
}
void QuantumNotImpl::s(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 's.body'");
}
void QuantumNotImpl::s(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 's.ctl'");
}
void QuantumNotImpl::s_adj(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 's.ctladj'");
}
void QuantumNotImpl::swap(Qubit, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'swap.body'");
}
void QuantumNotImpl::t_adj(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 't.adj'");
}
void QuantumNotImpl::t(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 't.body'");
}
void QuantumNotImpl::t(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 't.ctl'");
}
void QuantumNotImpl::t_adj(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 't.ctladj'");
}
void QuantumNotImpl::x(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'x.body'");
}
void QuantumNotImpl::x(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'x.ctl'");
}
void QuantumNotImpl::y(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'y.body'");
}
void QuantumNotImpl::y(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'y.ctl'");
}
void QuantumNotImpl::z(Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'z.body'");
}
void QuantumNotImpl::z(Array, Qubit)
{
    QIREE_NOT_IMPLEMENTED("quantum instruction 'z.ctl'");
}
void QuantumNotImpl::assertmeasurementprobability(
    Array, Array, Result, double, String, double)
{
    QIREE_NOT_IMPLEMENTED(
        "quantum instruction 'assertmeasurementprobability.body'");
}
void QuantumNotImpl::assertmeasurementprobability(Array, Tuple)
{
    QIREE_NOT_IMPLEMENTED(
        "quantum instruction 'assertmeasurementprobability.ctl'");
}

//---------------------------------------------------------------------------//
}  // namespace qiree
