//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/QsimQuantum.hh
//---------------------------------------------------------------------------//
#pragma once

#include <memory>
#include <ostream>
#include <vector>

#include "qiree/Macros.hh"
#include "qiree/QuantumNotImpl.hh"
#include "qiree/RuntimeInterface.hh"
#include "qiree/Types.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Create and execute quantum circuits using google Qsim.
 */
class QsimQuantum final : virtual public QuantumNotImpl
{
  public:
    // Construct with number of shots
    QsimQuantum(std::ostream& os, unsigned long int shots);
    ~QsimQuantum();

    QIREE_DELETE_COPY_MOVE(QsimQuantum);  // Delete copy and move constructors

    //!@{
    //! \name Accessors
    size_type num_results() const { return result_to_qubit_.size(); }
    size_type num_qubits() const { return num_qubits_; }
    //!@}

    //!@{
    //! \name Quantum interface
    // Prepare to build a quantum circuit for an entry point
    void set_up(EntryPointAttrs const&) override;

    // Complete an execution
    void tear_down() override;

    // Map a qubit to a result index
    void mz(Qubit, Result) final;

    // Read the value of a result.
    QState read_result(Result) final;
    //!@}

    //!@{
    //! \name Utilities for runtime
    // Get runtime qubit corresponding to a runtime result
    Qubit result_to_qubit(Result);

    // Run the circuit on the accelerator if we have not already. Returns true
    // if the circuit was executed.
    void execute_if_needed();

    void print_accelbuf();
    //!@}

    //!@{
    //! \name Circuit construction
    // void ccx(Qubit, Qubit) final;
    void ccnot(Qubit, Qubit, Qubit);  // TODO: not in examples or qir runner
    void cnot(Qubit, Qubit) final;
    void cx(Qubit, Qubit) final;
    // void cy(Qubit, Qubit) final;
    void cz(Qubit, Qubit) final;
    void h(Qubit) final;
    void reset(Qubit) final;
    void rx(double, Qubit) final;
    void ry(double, Qubit) final;
    void rz(double, Qubit) final;
    // void rzz(double, Qubit, Qubit) final;
    void s(Qubit) final;
    // void s_adj(Qubit) final;
    // void swap(Qubit, Qubit) final;
    void t(Qubit) final;
    // void t_adj(Qubit) final;
    void x(Qubit) final;
    void y(Qubit) final;
    void z(Qubit) final;
    //!@}

  private:

    //// TYPES ////

    struct Factory;
    struct State;

    //// DATA ////

    std::ostream& output_;
    unsigned long int seed_{};
    std::unique_ptr<State> state_;

    unsigned num_threads_{};  // Number of threads to use
    size_t gate_index_;  // when the quantum operation will be executed
    size_type num_qubits_{};
    std::vector<Qubit> result_to_qubit_;
};

}  // namespace qiree
