//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/QuantumInterface.hh
//---------------------------------------------------------------------------//
#pragma once

#include "Types.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Interface class for quantum instruction set in the \c qis (quantum)
 namespace.
 *
 * This class should subsume all known Quantum Instruction Set (QIS)
 * definitions. The QIR specification does not explicitly list these
 * operations...
 *
 * An instance of this class should be stateful (i.e. set up, store, and
 * measure the quantum circuit).
 *
 * TODO: at the beginning of an "entry point" the quantum device should have a
 * state initialized to zero.
 *
 * These QIR functions:
 * \verbatim
void @__quantum__qis__mz__body(%Qubit*, %Result*)
bool @__quantum__qis__read_result__body(%Result*)
void @__quantum__qis__r__ctl(%Array*, %Tuple*)
void @__quantum__qis__r__adj(i2, double, %Qubit*)
 * \endverbatim
 * are represented in this interface as
 * \verbatim
void mz(Qubit, Result);
QState read_result(Result);
void r(Array, Tuple);  //!< ctl
void r_adj(Pauli, double, Qubit);  //!< adj
 * \endverbatim
 *
 * \note These are generated from scripts/dev/generate-bindings.py .
 */
class QuantumInterface
{
  public:
    //@{
    //! \name Executor setup/teardown
    //! Prepare to build a quantum circuit for an entry point
    virtual void set_up(EntryPointAttrs const&) = 0;
    //! Complete an execution
    virtual void tear_down() = 0;
    //@}

    //@{
    //! \name Measurements

    virtual Result m(Qubit) = 0;  //!< body
    virtual Result measure(Array, Array) = 0;  //!< body
    virtual Result mresetz(Qubit) = 0;  //!< body
    virtual void mz(Qubit, Result) = 0;  //!< body
    virtual QState read_result(Result) = 0;  //!< body

    //@}
    //@{
    //! \name Gates

    virtual void ccx(Qubit, Qubit, Qubit) = 0;  //!< body
    virtual void cnot(Qubit, Qubit) = 0;  //!< body
    virtual void cx(Qubit, Qubit) = 0;  //!< body
    virtual void cy(Qubit, Qubit) = 0;  //!< body
    virtual void cz(Qubit, Qubit) = 0;  //!< body
    virtual void exp_adj(Array, double, Array) = 0;  //!< adj
    virtual void exp(Array, double, Array) = 0;  //!< body
    virtual void exp(Array, Tuple) = 0;  //!< ctl
    virtual void exp_adj(Array, Tuple) = 0;  //!< ctladj
    virtual void h(Qubit) = 0;  //!< body
    virtual void h(Array, Qubit) = 0;  //!< ctl
    virtual void r_adj(Pauli, double, Qubit) = 0;  //!< adj
    virtual void r(Pauli, double, Qubit) = 0;  //!< body
    virtual void r(Array, Tuple) = 0;  //!< ctl
    virtual void r_adj(Array, Tuple) = 0;  //!< ctladj
    virtual void reset(Qubit) = 0;  //!< body
    virtual void rx(double, Qubit) = 0;  //!< body
    virtual void rx(Array, Tuple) = 0;  //!< ctl
    virtual void rxx(double, Qubit, Qubit) = 0;  //!< body
    virtual void ry(double, Qubit) = 0;  //!< body
    virtual void ry(Array, Tuple) = 0;  //!< ctl
    virtual void ryy(double, Qubit, Qubit) = 0;  //!< body
    virtual void rz(double, Qubit) = 0;  //!< body
    virtual void rz(Array, Tuple) = 0;  //!< ctl
    virtual void rzz(double, Qubit, Qubit) = 0;  //!< body
    virtual void s_adj(Qubit) = 0;  //!< adj
    virtual void s(Qubit) = 0;  //!< body
    virtual void s(Array, Qubit) = 0;  //!< ctl
    virtual void s_adj(Array, Qubit) = 0;  //!< ctladj
    virtual void swap(Qubit, Qubit) = 0;  //!< body
    virtual void t_adj(Qubit) = 0;  //!< adj
    virtual void t(Qubit) = 0;  //!< body
    virtual void t(Array, Qubit) = 0;  //!< ctl
    virtual void t_adj(Array, Qubit) = 0;  //!< ctladj
    virtual void x(Qubit) = 0;  //!< body
    virtual void x(Array, Qubit) = 0;  //!< ctl
    virtual void y(Qubit) = 0;  //!< body
    virtual void y(Array, Qubit) = 0;  //!< ctl
    virtual void z(Qubit) = 0;  //!< body
    virtual void z(Array, Qubit) = 0;  //!< ctl

    //@}
    //@{
    //! \name Assertions

    virtual void
    assertmeasurementprobability(Array, Array, Result, double, String, double)
        = 0;  //!< body
    virtual void assertmeasurementprobability(Array, Tuple) = 0;  //!< ctl

    //@}

  protected:
    virtual ~QuantumInterface() = default;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
