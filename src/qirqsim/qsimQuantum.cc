//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/qsimQuantum.cc
//---------------------------------------------------------------------------//

#include "qsimQuantum.hh"

#include <algorithm>
#include <iostream>
#include <utility>
#include <stdexcept>
#include <thread>
#include <optional>
#include <cassert>

#include "qiree/Assert.hh"

// Qsim
#include "../../tpls/qsim/simulator_basic.h"
#include "../../tpls/qsim/statespace_basic.h"
#include "../../tpls/qsim/gates_qsim.h"
#include "../../tpls/qsim/circuit.h"
#include "../../tpls/qsim/run_qsim.h"
#include "../../tpls/qsim/io.h"
#include "../../tpls/qsim/fuser.h"
#include "../../tpls/qsim/circuit_qsim_parser.h" 
#include "../../tpls/qsim/fuser_mqubit.h"
#include "../../tpls/qsim/io_file.h"
#include "../../tpls/qsim/simmux.h"
#include "../../tpls/qsim/util_cpu.h"
#include "../../tpls/qsim/formux.h"
#include "../../tpls/qsim/gate.h"
//

namespace qiree{
//---------------------------------------------------------------------------//
/*
Initialize the qsim simulator
*/

qsimQuantum::State qsimQuantum::init_state_space() { //check if StateSpace is the proper type for the output, problably it is just State from the Fatory struct.
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator
    qsimParam.seed = std::rand(); // Set the seed for qsim parameters
    numThreads = std::max(1, static_cast<int>(std::thread::hardware_concurrency())); // Get the number of threads
    qsimParam.max_fused_size = 2; // Set the maximum size of fused gates
    qsimParam.verbosity = 0; // see verbosity in run_qsim.h 
    // Initialize the qsim simulator
    qsimQuantum::StateSpace state_space = Factory(numThreads).CreateStateSpace(); // Create the state space
    State state = state_space.Create(this->num_qubits()); // Create the state
    // Check if the state is null
    if (state_space.IsNull(state)) {
        qsim::IO::errorf("not enough memory: is the number of qubits too large?\n");
    }
    state_space.SetStateZero(state); // Set the state to zero, TODO: the initial state is not necessarily zero
  return state;
  }
  
  qsimQuantum::qsimQuantum(std::ostream& os,
                 size_type shots)
  : output_(os)
  {
  }

//---------------------------------------------------------------------------//
/*
Prepare to build a quantum circuit for an entry point
*/

void qsimQuantum::set_up(EntryPointAttrs const& attrs) {
    QIREE_VALIDATE(attrs.required_num_qubits > 0,
                   << "input is not a quantum program");
    // Resize the result_to_qubit_ vector, based on the required number of results...
    // the idea is to have as many classical registers as qubits (probably not true in general)
    result_to_qubit_.resize(attrs.required_num_results);
    num_qubits_ = attrs.required_num_qubits; // Set the number of qubits
    state_ = std::make_shared<State>(init_state_space()); // Set the state space? Maybe.
    q_circuit.num_qubits = num_qubits_; // Allocate the number of qubits in the circuit
    execution_time = 0; // Initialize execution time
    static unsigned int rep = 0;
    rep++;
    this->repCount(rep);
}

//---------------------------------------------------------------------------//
/*
Complete an execution
*/

void qsimQuantum::repCount(int rep) {
    repetition = rep;
}

void qsimQuantum::tear_down() {
    q_circuit = {};
    q_circuit.num_qubits = num_qubits_;
    state_ = std::make_shared<State>(init_state_space());
}

//---------------------------------------------------------------------------//
/*
Reset the qubit
*/

void qsimQuantum::reset(Qubit q) {
    q.value=0;
}

//----------------------------------------------------------------------------//
/* 
Read the value of a result. This utilizes the new BufferManager.
*/

QState qsimQuantum::read_result(Result r)
{
    std::string q_index_string = std::to_string(r.value);
    auto meas_results = execute_if_needed();
    if (meas_results.size() == 1 && meas_results[0].bitstring.size() == 1) {
        const auto bitResult = meas_results[0].bitstring[0];
        assert(bitResult == 0 || bitResult == 1);
        std::string stringResult = std::to_string(bitResult);
        if (stringResult == "1"){
            manager.updateBuffer("q"+q_index_string, "1", 1);
            manager.updateBuffer("q"+q_index_string, 1);
        } else{
            manager.updateBuffer("q"+q_index_string, "0", 1);
            manager.updateBuffer("q"+q_index_string, 0);
        }
    } else {
        qsim::IO::errorf("Unexpected measurement results encountered.");
    }
    return static_cast<QState>(meas_results[0].bitstring[0]);
}

//---------------------------------------------------------------------------//
/*
Map a qubit to a result index 
(TODO: find how to link the classical register to the quantum register in qsim)
*/

void qsimQuantum::mz(Qubit q, Result r) { //we don't classical register yet. 
    QIREE_EXPECT(q.value < this->num_qubits()); // TODO: q must be in the set of qubits, e.g., what happens if q=5 and qubits are {2,3,4,5}, q is less than num_qubits but not it is in the set of qubits. 
    // Add measurement instruction
    this->q_circuit.gates.push_back(
        qsim::gate::Measurement<qsim::GateQSim<float>>::Create(
            execution_time++, {this->getQubitIndex(q)}));
}

//---------------------------------------------------------------------------//
/*
Quantum Instruction Mapping
*/

// 1. Entangling gates
void qsimQuantum::cx(Qubit q1, Qubit q2) {
    q_circuit.gates.push_back(
        qsim::GateCNot<float>::Create(execution_time++, this->getQubitIndex(q1), this->getQubitIndex(q2)));
}
void qsimQuantum::cnot(Qubit q1, Qubit q2) {
    q_circuit.gates.push_back(
        qsim::GateCNot<float>::Create(execution_time++, this->getQubitIndex(q1), this->getQubitIndex(q2)));
}
void qsimQuantum::cz(Qubit q1, Qubit q2) {
    q_circuit.gates.push_back(
        qsim::GateCZ<float>::Create(execution_time++, this->getQubitIndex(q1), this->getQubitIndex(q2)));
}
// 2. Local gates
void qsimQuantum::h(Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateHd<float>::Create(execution_time++, this->getQubitIndex(q)));
}
void qsimQuantum::s(Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateS<float>::Create(execution_time++, this->getQubitIndex(q)));
}
void qsimQuantum::t(Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateT<float>::Create(execution_time++, this->getQubitIndex(q)));
}
// 2.1 Pauli gates
void qsimQuantum::x(Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateX<float>::Create(execution_time++, this->getQubitIndex(q)));
}
void qsimQuantum::y(Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateY<float>::Create(execution_time++, this->getQubitIndex(q)));
}
void qsimQuantum::z(Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateZ<float>::Create(execution_time++, this->getQubitIndex(q)));
}
// 2.2 rotation gates
void qsimQuantum::rx(double theta, Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateRX<float>::Create(execution_time++, this->getQubitIndex(q), theta));
}
void qsimQuantum::ry(double theta, Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateRY<float>::Create(execution_time++, this->getQubitIndex(q), theta));
}
void qsimQuantum::rz(double theta, Qubit q) {
    q_circuit.gates.push_back(
        qsim::GateRZ<float>::Create(execution_time++, this->getQubitIndex(q), theta));
}

Qubit qsimQuantum::result_to_qubit(Result r) {
    // TODO: This function is not working. Giving 0 every time. Maybe not needed.
    QIREE_EXPECT(r.value < this->num_results());
    return result_to_qubit_[r.value]; // just copied this from the qirxacc, I have no idea if we need to do something else here
}

void qsimQuantum::print_accelbuf() {
    // TODO: to be implemented, we can create a buffer class to store the results
}

qsimQuantum::VecMeas qsimQuantum::execute_if_needed() {
    std::vector<StateSpace::MeasurementResult> meas_results; // Vector to hold measurement results, this must be empty before running
    std::string stringResult;
    static unsigned long int seed = 0;
    qsimParam.seed = seed++;
    const bool run_success = Runner::Run(qsimParam, Factory(numThreads), q_circuit, *state_, meas_results); // Run the simulation
    assert(run_success); // Ensure the run was successful
	// reset circuit here 
	q_circuit = {};
    q_circuit.num_qubits = num_qubits_;
    return meas_results;
}

} // namespace qiree
