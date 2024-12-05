//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/QuantumNotImpl.hh
//---------------------------------------------------------------------------//
#pragma once

#include "QuantumInterface.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Mixin class for raising an error on a not-implemented instruction.
 */
class QuantumNotImpl : virtual public QuantumInterface
{
  public:
    //@{
    //! \name Measurements

    Result m(Qubit) override;
    Result measure(Array, Array) override;
    Result mresetz(Qubit) override;
    void mz(Qubit, Result) override;
    QState read_result(Result) override;

    //@}
    //@{
    //! \name Gates

    void ccx(Qubit, Qubit, Qubit) override;
    void cnot(Qubit, Qubit) override;
    void cx(Qubit, Qubit) override;
    void cy(Qubit, Qubit) override;
    void cz(Qubit, Qubit) override;
    void exp_adj(Array, double, Array) override;
    void exp(Array, double, Array) override;
    void exp(Array, Tuple) override;
    void exp_adj(Array, Tuple) override;
    void h(Qubit) override;
    void h(Array, Qubit) override;
    void r_adj(Pauli, double, Qubit) override;
    void r(Pauli, double, Qubit) override;
    void r(Array, Tuple) override;
    void r_adj(Array, Tuple) override;
    void reset(Qubit) override;
    void rx(double, Qubit) override;
    void rx(Array, Tuple) override;
    void rxx(double, Qubit, Qubit) override;
    void ry(double, Qubit) override;
    void ry(Array, Tuple) override;
    void ryy(double, Qubit, Qubit) override;
    void rz(double, Qubit) override;
    void rz(Array, Tuple) override;
    void rzz(double, Qubit, Qubit) override;
    void s_adj(Qubit) override;
    void s(Qubit) override;
    void s(Array, Qubit) override;
    void s_adj(Array, Qubit) override;
    void swap(Qubit, Qubit) override;
    void t_adj(Qubit) override;
    void t(Qubit) override;
    void t(Array, Qubit) override;
    void t_adj(Array, Qubit) override;
    void x(Qubit) override;
    void x(Array, Qubit) override;
    void y(Qubit) override;
    void y(Array, Qubit) override;
    void z(Qubit) override;
    void z(Array, Qubit) override;

    //@}
    //@{
    //! \name Assertions

    void assertmeasurementprobability(
        Array, Array, Result, double, String, double) override;
    void assertmeasurementprobability(Array, Tuple) override;
    //
    //@}
};

//---------------------------------------------------------------------------//
}  // namespace qiree
