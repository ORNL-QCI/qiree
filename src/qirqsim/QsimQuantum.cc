//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/QsimQuantum.cc
//---------------------------------------------------------------------------//

#include "QsimQuantum.hh"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <thread>
#include <utility>

#include "qiree/Assert.hh"

// Qsim
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
//

namespace qiree
{

struct Factory
{  // Factory class for creating simulators in qsim
    Factory(unsigned num_threads) : num_threads(num_threads) {}
    using Simulator = qsim::Simulator<qsim::For>;
    using StateSpace = Simulator::StateSpace;
    StateSpace CreateStateSpace() const { return StateSpace(num_threads); }
    Simulator CreateSimulator() const { return Simulator(num_threads); }
    unsigned num_threads;
};

//---------------------------------------------------------------------------//
/*!
 * Initialize the qsim simulator
 */
QsimQuantum::QsimQuantum(std::ostream& os, unsigned long int seed)
    : output_(os), seed_(seed)
{
}

//---------------------------------------------------------------------------//
/*!
 * Prepare to build a quantum circuit for an entry point
 */
void QsimQuantum::set_up(EntryPointAttrs const& attrs)
{
    QIREE_VALIDATE(attrs.required_num_qubits > 0,
                   << "input is not a quantum program");
    // Resize the result_to_qubit_ vector, based on the required number of
    // results... the idea is to have as many classical registers as qubits
    // (probably not true in general)
    result_to_qubit_.resize(attrs.required_num_results);
    num_qubits_ = attrs.required_num_qubits;  // Set the number of qubits

    // Get the number of threads
    num_threads_
        = std::max(1, static_cast<int>(std::thread::hardware_concurrency()));

    // Initialize the qsim simulator
    QsimQuantum::StateSpace state_space
        = Factory(num_threads_).CreateStateSpace();  // Create the state space

    // Create the state
    State state = state_space.Create(this->num_qubits());
    // Check if the state is null
    QIREE_VALIDATE(!state_space.IsNull(state),
                   << "not enough memory: is the number of qubits too large?");

    state_space.SetStateZero(state);  // Set the state to zero, TODO: the
                                      // initial state is not necessarily zero

    state_ = std::make_shared<State>(std::move(state));

    // Allocate the number of qubits in the circuit
    q_circuit.num_qubits = num_qubits_;
    gate_index_ = 0;  // Initialize execution time
}

//---------------------------------------------------------------------------//
/*!
 * Complete an execution
 */
void QsimQuantum::tear_down()
{
    q_circuit = {};
}

//---------------------------------------------------------------------------//
/*!
 * Reset the qubit
 */
void QsimQuantum::reset(Qubit q)
{
    q.value = 0;
}

//----------------------------------------------------------------------------//
/*!
 * Read the value of a result. This utilizes the new BufferManager.
 */
QState QsimQuantum::read_result(Result r)
{
    using Fuser = qsim::MultiQubitGateFuser<qsim::IO, qsim::GateQSim<float>>;
    using Runner = qsim::QSimRunner<qsim::IO, Fuser, Factory>;
    using VecMeas = std::vector<StateSpace::MeasurementResult>;

    // Vector to hold measurement results, this must be empty before running
    std::vector<StateSpace::MeasurementResult> meas_results;
    std::string stringResult;

    Runner::Parameter qsimParam;  // Parameters for qsim
    qsimParam.seed = seed_;
    seed_++;
    qsimParam.max_fused_size = 2;  // Set the maximum size of fused gates
    qsimParam.verbosity = 0;  // see verbosity in run_qsim.h

    // Run the simulation
    bool const run_success = Runner::Run(
        qsimParam, Factory(num_threads_), q_circuit, *state_, meas_results);

    assert(run_success);  // Ensure the run was successful
    // reset circuit here
    q_circuit = {};
    q_circuit.num_qubits = num_qubits_;

    if (meas_results.size() == 1 && meas_results[0].bitstring.size() == 1)
    {
        auto const bitResult = meas_results[0].bitstring[0];
        assert(bitResult == 0 || bitResult == 1);
        std::string stringResult = std::to_string(bitResult);
        std::string q_index_string = std::to_string(r.value);
        if (stringResult == "1")
        {
            manager.updateBuffer("q" + q_index_string, "1", 1);
            manager.updateBuffer("q" + q_index_string, 1);
        }
        else
        {
            manager.updateBuffer("q" + q_index_string, "0", 1);
            manager.updateBuffer("q" + q_index_string, 0);
        }
    }
    else
    {
        qsim::IO::errorf("Unexpected measurement results encountered.");
    }
    return static_cast<QState>(meas_results[0].bitstring[0]);
}

//---------------------------------------------------------------------------//
/*!
 * Map a qubit to a result index.
 *
 * (TODO: find how to link the classical register to the quantum register in
 * qsim)
 */
void QsimQuantum::mz(Qubit q, Result r)
{  // we don't classical register yet.
    QIREE_EXPECT(q.value < this->num_qubits());  // TODO: q must be in the set
                                                 // of qubits, e.g., what
                                                 // happens if q=5 and qubits
                                                 // are {2,3,4,5}, q is less
                                                 // than num_qubits but not it
                                                 // is in the set of qubits.
    // TODO: maybe not what we want long term
    QIREE_EXPECT(q.value == r.value);
    // Add measurement instruction
    this->q_circuit.gates.push_back(
        qsim::gate::Measurement<qsim::GateQSim<float>>::Create(gate_index_++,
                                                               {q.value}));
}

//---------------------------------------------------------------------------//
/*
 * Quantum Instruction Mapping
 */

// 1. Entangling gates
void QsimQuantum::cx(Qubit q1, Qubit q2)
{
    q_circuit.gates.push_back(
        qsim::GateCNot<float>::Create(gate_index_++, q1.value, q2.value));
}
void QsimQuantum::cnot(Qubit q1, Qubit q2)
{
    q_circuit.gates.push_back(
        qsim::GateCNot<float>::Create(gate_index_++, q1.value, q2.value));
}
void QsimQuantum::cz(Qubit q1, Qubit q2)
{
    q_circuit.gates.push_back(
        qsim::GateCZ<float>::Create(gate_index_++, q1.value, q2.value));
}
// 2. Local gates
void QsimQuantum::h(Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateHd<float>::Create(gate_index_++, q.value));
}
void QsimQuantum::s(Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateS<float>::Create(gate_index_++, q.value));
}
void QsimQuantum::t(Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateT<float>::Create(gate_index_++, q.value));
}
// 2.1 Pauli gates
void QsimQuantum::x(Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateX<float>::Create(gate_index_++, q.value));
}
void QsimQuantum::y(Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateY<float>::Create(gate_index_++, q.value));
}
void QsimQuantum::z(Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateZ<float>::Create(gate_index_++, q.value));
}
// 2.2 rotation gates
void QsimQuantum::rx(double theta, Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateRX<float>::Create(gate_index_++, q.value, theta));
}
void QsimQuantum::ry(double theta, Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateRY<float>::Create(gate_index_++, q.value, theta));
}
void QsimQuantum::rz(double theta, Qubit q)
{
    q_circuit.gates.push_back(
        qsim::GateRZ<float>::Create(gate_index_++, q.value, theta));
}

Qubit QsimQuantum::result_to_qubit(Result r)
{
    // TODO: This function is not working. Giving 0 every time. Maybe not
    // needed.
    QIREE_EXPECT(r.value < this->num_results());
    return result_to_qubit_[r.value];  // just copied this from the qirxacc, I
                                       // have no idea if we need to do
                                       // something else here
}

void QsimQuantum::print_accelbuf()
{
    // TODO: to be implemented, we can create a buffer class to store the
    // results
}

void QsimQuantum::execute_if_needed()
{
    QIREE_EXPECT(false);
}

}  // namespace qiree
