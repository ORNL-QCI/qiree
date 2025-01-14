//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirlightning/LightningQuantum.cc
//---------------------------------------------------------------------------//

#include "LightningQuantum.hh"

#include <algorithm>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <thread>
#include <utility>

#include "qiree/Assert.hh"

// Lightning
#include "catalyst_runtime/lib/capi/ExecutionContext.hpp"

namespace qiree
{
using namespace Catalyst::Runtime;

static inline std::shared_ptr<RTDevice> loadRTDevice(const std::string &rtd_lib,
                                                   const std::string &rtd_name = {},
                                                   const std::string &rtd_kwargs = {})
{
    ExecutionContext context;
    return context.getOrCreateDevice(rtd_lib, rtd_name, rtd_kwargs);
}

//---------------------------------------------------------------------------//
/*!
 * Initialize the Lightning simulator
 */
LightningQuantum::LightningQuantum(std::ostream& os, unsigned long int seed)
{
    auto RTDevice = loadDevice("/home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages/pennylane_lightning/liblightning_gpu_catalyst.so", "LightningGPUSimulator", "");

}

//---------------------------------------------------------------------------//
//! Default destructor
LightningQuantum::~LightningQuantum() = default;

//---------------------------------------------------------------------------//
/*!
 * Prepare to build a quantum circuit for an entry point
 */
void LightningQuantum::set_up(EntryPointAttrs const& attrs)
{
    QIREE_VALIDATE(attrs.required_num_qubits > 0,
                   << "input is not a quantum program");
    
    num_qubits_ = attrs.required_num_qubits;  // Set the number of qubits

    RTDevice->getQuantumDevicePtr()->AllocateQubits(num_qubits_);

}

//---------------------------------------------------------------------------//
/*!
 * Complete an execution
 */
void LightningQuantum::tear_down()
{
    context->deactivateDevice(RTDevice);
    RTDevice = nullptr;
}

//---------------------------------------------------------------------------//
/*!
 * Reset the qubit
 */
void LightningQuantum::reset(Qubit q)
{
    q.value = 0;
}

//----------------------------------------------------------------------------//
/*!
 * Read the value of a result. This utilizes the new BufferManager.
 */
QState LightningQuantum::read_result(Result r)
{

    return static_cast<QState>(meas_results[0].bitstring[0]);
}

//---------------------------------------------------------------------------//
/*!
 * Map a qubit to a result index.
 *
 * (TODO: find how to link the classical register to the quantum register in
 * qsim)
 */
void LightningQuantum::mz(Qubit q, Result r)
{  // we don't classical register yet.
    /* QIREE_EXPECT(q.value < this->num_qubits()); */  // TODO: q must be in the set
                                                 // of qubits, e.g., what
                                                 // happens if q=5 and qubits
                                                 // are {2,3,4,5}, q is less
                                                 // than num_qubits but not it
                                                 // is in the set of qubits.
    // TODO: maybe not what we want long term
    QIREE_EXPECT(q.value == r.value);
    // Add measurement instruction
    Measure(q.value, std::nullopt);
    // RETURN MEASURE RESULT??

}

//---------------------------------------------------------------------------//
/*
 * Quantum Instruction Mapping
 */

// 1. Entangling gates
void LightningQuantum::cx(Qubit q1, Qubit q2)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("CNOT", {}, {q1.value, q2.value});
}
void LightningQuantum::cnot(Qubit q1, Qubit q2)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("CNOT", {}, {q1.value, q2.value});
}
void LightningQuantum::cz(Qubit q1, Qubit q2)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("CZ", {}, {q1.value, q2.value});
}
// 2. Local gates
void LightningQuantum::h(Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("Hadamard", {}, {q.value});
}
void LightningQuantum::s(Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("S", {}, {q.value});
}
void LightningQuantum::t(Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("T", {}, {q.value});
}
// 2.1 Pauli gates
void LightningQuantum::x(Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("PauliX", {}, {q.value});
}
void LightningQuantum::y(Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("PauliY", {}, {q.value});
}
void LightningQuantum::z(Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("PauliZ", {}, {q.value});
}
// 2.2 rotation gates
void LightningQuantum::rx(double theta, Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("RX", {theta}, {q.value});
}
void LightningQuantum::ry(double theta, Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("RY", {theta}, {q.value});
}
void LightningQuantum::rz(double theta, Qubit q)
{
    RTDevice->getQuantumDevicePtr()->NamedOperation("RZ", {theta}, {q.value});
}

Qubit LightningQuantum::result_to_qubit(Result r)
{
    // TODO: This function is not working. Giving 0 every time. Maybe not
    // needed.
    /* QIREE_EXPECT(r.value < this->num_results()); */
    return result_to_qubit_[r.value];  // just copied this from the qirxacc, I
                                       // have no idea if we need to do
                                       // something else here
}

void LightningQuantum::print_accelbuf()
{
    // TODO: to be implemented, we can create a buffer class to store the
    // results
}

void LightningQuantum::execute_if_needed()
{
    /* QIREE_EXPECT(false); */
}

}  // namespace qiree
