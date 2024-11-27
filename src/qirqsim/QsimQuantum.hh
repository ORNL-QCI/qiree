//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/QsimQuantum.hh
//---------------------------------------------------------------------------//
#pragma once

#include <cassert>
#include <initializer_list>
#include <map>
#include <memory>
#include <ostream>
#include <vector>
#include <qsim/lib/circuit.h>
#include <qsim/lib/circuit_qsim_parser.h>
#include <qsim/lib/formux.h>
#include <qsim/lib/fuser.h>
#include <qsim/lib/fuser_mqubit.h>
#include <qsim/lib/gate.h>
#include <qsim/lib/gates_qsim.h>
#include <qsim/lib/io.h>
#include <qsim/lib/io_file.h>
#include <qsim/lib/run_qsim.h>
#include <qsim/lib/simmux.h>
#include <qsim/lib/simulator_basic.h>
#include <qsim/lib/statespace_basic.h>
#include <qsim/lib/util_cpu.h>

#include "BufferManager.hh"
#include "qiree/Macros.hh"
#include "qiree/QuantumNotImpl.hh"
#include "qiree/RuntimeInterface.hh"
#include "qiree/Types.hh"

struct Factory
{  // Factory class for creating simulators in qsim
    Factory(unsigned num_threads) : num_threads(num_threads) {}
    using Simulator = qsim::Simulator<qsim::For>;
    using StateSpace = Simulator::StateSpace;
    StateSpace CreateStateSpace() const { return StateSpace(num_threads); }
    Simulator CreateSimulator() const { return Simulator(num_threads); }
    unsigned num_threads;
};

namespace qiree
{
class QsimQuantum final : virtual public QuantumNotImpl
{
  public:
    // Define constructors and destructors
    // Construct with number of shots
    QsimQuantum(std::ostream& os, size_type shots);  

    // Define types
    using Simulator = qsim::Simulator<qsim::For>;
    using StateSpace = Simulator::StateSpace;
    using State = StateSpace::State;
    using Fuser = qsim::MultiQubitGateFuser<qsim::IO, qsim::GateQSim<float>>;
    using Runner = qsim::QSimRunner<qsim::IO, Fuser, Factory>;
    using VecMeas = std::vector<StateSpace::MeasurementResult>;

    State init_state_space();

    QIREE_DELETE_COPY_MOVE(QsimQuantum);  // Delete copy and move constructors

    //!@{
    //! \name Accessors
    size_type num_results() const { return result_to_qubit_.size(); }
    size_type num_qubits() const { return num_qubits_; }

    unsigned getQubitIndex(Qubit q)
    {
        return static_cast<unsigned>(q.value);  // Return the value of the
                                                // qubit
    }
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

    // Wrapper for qsim
    // std::map<std::string, int>
    // get_marginal_counts(std::vector<Qubit> const& qubits);

    // Run the circuit on the accelerator if we have not already. Returns true
    // if the circuit was executed.
    VecMeas execute_if_needed();

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

    // Get the quantum circuit
    qsim::Circuit<qsim::GateQSim<float>> get_circuit() const
    {
        return q_circuit;
    }
    // Get the state space
    State const& get_state() const { return *state_; }
    // Update the buffer
    BufferManager manager;
    // Number of repetitions
    int repetition;
    void repCount(int rep);

  private:
    //// TYPES ////
    enum class Endianness
    {
        little,
        big
    };
    unsigned numThreads;  // Number of threads to use
    unsigned max_fused_size;  // Maximum size of fused gates
    qsim::Circuit<qsim::GateQSim<float>> q_circuit;  // Quantum circuit object

    Runner::Parameter qsimParam;  // Parameters for qsim
    unsigned long int seed_;
    size_t execution_time;  // when the quantum operation will be executed

    bool executed;
    size_type num_qubits_{};
    std::vector<Qubit> result_to_qubit_;
    Endianness endian_;

    std::ostream& output_;
    std::shared_ptr<Simulator> simulator_;
    std::shared_ptr<StateSpace> statespace_;
    std::shared_ptr<State> state_;
};

class buffer
{
  public:
    buffer(size_t size) : size(size) {}
    size_t size;
};

}  // namespace qiree