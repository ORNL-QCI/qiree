//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/QsimQuantum.cc
//---------------------------------------------------------------------------//

#include "QsimQuantum.hh"

#include <algorithm>
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
//---------------------------------------------------------------------------//
/*!
 * Factory class for creating simulators in qsim.
 */
struct QsimQuantum::Factory
{
    Factory(unsigned num_threads) : num_threads(num_threads) {}
    using Simulator = qsim::Simulator<qsim::For>;
    using StateSpace = Simulator::StateSpace;

    StateSpace CreateStateSpace() const { return StateSpace(num_threads); }
    Simulator CreateSimulator() const { return Simulator(num_threads); }
    unsigned num_threads;
};

//---------------------------------------------------------------------------//
/*!
 * Quantum state and circuit.
 */
struct QsimQuantum::State
{
    qsim::Circuit<qsim::GateQSim<float>> circuit;
    std::optional<Factory::StateSpace::State> state;
};

//---------------------------------------------------------------------------//
/*!
 * Initialize the qsim simulator
 */
QsimQuantum::QsimQuantum(std::ostream& os, unsigned long int seed)
    : output_(os), seed_(seed), state_{std::make_unique<State>()}
{
}

//---------------------------------------------------------------------------//
//! Default destructor
QsimQuantum::~QsimQuantum() = default;

//---------------------------------------------------------------------------//
/*!
 * Prepare to build a quantum circuit for an entry point.
 */
void QsimQuantum::set_up(EntryPointAttrs const& attrs)
{
    QIREE_VALIDATE(attrs.required_num_qubits > 0,
                   << "input is not a quantum program");

    // Resize the result_to_qubit_ vector, based on the required number of
    // results... the idea is to have as many classical registers as qubits
    // (probably not true in general)
    results_.resize(attrs.required_num_results);
    num_qubits_ = attrs.required_num_qubits;
    num_threads_
        = std::max(1, static_cast<int>(std::thread::hardware_concurrency()));

    // Initialize the qsim simulator
    auto state_space = Factory(num_threads_).CreateStateSpace();

    // Create the state
    state_->state = state_space.Create(this->num_qubits());
    // Check if the state is null
    QIREE_VALIDATE(!state_space.IsNull(*state_->state),
                   << "not enough memory: is the number of qubits too large?");

    // TODO: initial states shouldn't necessarily be zero
    state_space.SetStateZero(*state_->state);

    // Allocate the number of qubits in the circuit
    state_->circuit.num_qubits = num_qubits_;
    gate_index_ = 0;
}

//---------------------------------------------------------------------------//
/*!
 * Complete an execution.
 */
void QsimQuantum::tear_down()
{
    state_->circuit = {};
}

//---------------------------------------------------------------------------//
/*!
 * Reset the qubit
 */
void QsimQuantum::reset(Qubit q)
{
    q.value = 0;
}

//---------------------------------------------------------------------------//
/*!
 * Map a qubit to a result index.
 *
 * (TODO: find how to link the classical register to the quantum register in
 * qsim)
 */
void QsimQuantum::mz(Qubit q, Result r)
{
    QIREE_EXPECT(q.value < this->num_qubits());
    QIREE_EXPECT(r.value < this->num_results());

    // Add measurement instruction
    state_->circuit.gates.push_back(
        qsim::gate::Measurement<qsim::GateQSim<float>>::Create(
            gate_index_++, {static_cast<unsigned int>(q.value)}));

    //// EXECUTE CIRCUIT ////

    using Fuser = qsim::MultiQubitGateFuser<qsim::IO, qsim::GateQSim<float>>;
    using Runner = qsim::QSimRunner<qsim::IO, Fuser, Factory>;
    using StateSpace = Factory::StateSpace;

    // Vector to hold measurement results, this must be empty before running
    std::vector<StateSpace::MeasurementResult> meas_results;
    std::string stringResult;

    Runner::Parameter qsimParam;  // Parameters for qsim
    qsimParam.seed = seed_;
    seed_++;
    qsimParam.max_fused_size = 2;  // Set the maximum size of fused gates
    qsimParam.verbosity = 0;  // see verbosity in run_qsim.h

    // Run the simulation and check that it passed
    bool const run_success = Runner::Run(qsimParam,
                                         Factory(num_threads_),
                                         state_->circuit,
                                         *state_->state,
                                         meas_results);
    QIREE_ASSERT(run_success);
    QIREE_VALIDATE(
        meas_results.size() == 1 && meas_results[0].bitstring.size() == 1,
        << "inconsistent measured results size (" << meas_results.size()
        << "), bitstring size");

    //// RESET CIRCUIT ////

    state_->circuit = {};
    state_->circuit.num_qubits = num_qubits_;

    //// STORE RESULT ////

    auto result = meas_results[0].bitstring[0];
    QIREE_ASSERT(result == 0 || result == 1);

    results_[r.value] = result;
}

//----------------------------------------------------------------------------//
/*!
 * Read the value of a result.
 *
 * \todo We could add assertions to check that we actually measured into the
 * given result.
 */
QState QsimQuantum::read_result(Result r) const
{
    QIREE_EXPECT(r.value < results_.size());
    auto result_bool = static_cast<bool>(results_[r.value]);
    return static_cast<QState>(result_bool);
}

//---------------------------------------------------------------------------//
//// Entangling gates ////
void QsimQuantum::cx(Qubit q1, Qubit q2)
{
    this->add_gate<qsim::GateCNot>(q1.value, q2.value);
}
void QsimQuantum::cnot(Qubit q1, Qubit q2)
{
    this->add_gate<qsim::GateCNot>(q1.value, q2.value);
}
void QsimQuantum::cz(Qubit q1, Qubit q2)
{
    this->add_gate<qsim::GateCZ>(q1.value, q2.value);
}

//// Local gates ////
void QsimQuantum::h(Qubit q)
{
    this->add_gate<qsim::GateHd>(q.value);
}
void QsimQuantum::s(Qubit q)
{
    this->add_gate<qsim::GateS>(q.value);
}
void QsimQuantum::t(Qubit q)
{
    this->add_gate<qsim::GateT>(q.value);
}

//// Pauli gates ////
void QsimQuantum::x(Qubit q)
{
    this->add_gate<qsim::GateX>(q.value);
}
void QsimQuantum::y(Qubit q)
{
    this->add_gate<qsim::GateY>(q.value);
}
void QsimQuantum::z(Qubit q)
{
    this->add_gate<qsim::GateZ>(q.value);
}

//// Rotation gates ////
void QsimQuantum::rx(double theta, Qubit q)
{
    this->add_gate<qsim::GateRX>(q.value, theta);
}
void QsimQuantum::ry(double theta, Qubit q)
{
    this->add_gate<qsim::GateRY>(q.value, theta);
}
void QsimQuantum::rz(double theta, Qubit q)
{
    this->add_gate<qsim::GateRZ>(q.value, theta);
}

//----------------------------------------------------------------------------//
// PRIVATE HELPERS
//----------------------------------------------------------------------------//
//! Create a gate and add it to the circuit
template<template<class> class Gate, class... Ts>
void QsimQuantum::add_gate(Ts&&... args)
{
    state_->circuit.gates.push_back(
        Gate<float>::Create(gate_index_++, std::forward<Ts>(args)...));
}

}  // namespace qiree
