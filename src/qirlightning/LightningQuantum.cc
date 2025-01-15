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
#include "QuantumDevice.hpp"

#define RTDLIB                                                         \
    "/home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages/" \
    "pennylane_lightning/liblightning_kokkos_catalyst.so";
#define RTDDEVICE "LightningKokkosSimulator";

namespace qiree
{
using namespace Catalyst::Runtime;

//---------------------------------------------------------------------------//
/*!
 * Initialize the Lightning simulator
 */
LightningQuantum::LightningQuantum(std::ostream& os) : output_(os)
{
    std::string rtd_lib = RTDLIB;
    std::string rtd_device = RTDDEVICE;
    std::string kwargs = {};
    auto rtld_flags = RTLD_LAZY | RTLD_NODELETE;
    auto rtd_dylib_handler = dlopen(rtd_lib.c_str(), rtld_flags);

    if (!rtd_dylib_handler)
    {
        throw std::runtime_error("Failed to load library: " + rtd_lib);
    }

    // Find device factory
    std::string factory_name = rtd_device + "Factory";
    void* f_ptr = dlsym(rtd_dylib_handler, factory_name.c_str());

    if (!f_ptr)
    {
        dlclose(rtd_dylib_handler);
        throw std::runtime_error("Failed to find factory function: "
                                 + factory_name);
    }
    std::string rtd_kwargs = {};
    rtd_qdevice = std::unique_ptr<QuantumDevice>(
        reinterpret_cast<decltype(GenericDeviceFactory)*>(f_ptr)(
            rtd_kwargs.c_str()));
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

    rtd_qdevice->AllocateQubits(num_qubits_);
}

//---------------------------------------------------------------------------//
/*!
 * Complete an execution
 */
void LightningQuantum::tear_down()
{
    if (rtd_dylib_handler)
    {
        dlclose(rtd_dylib_handler);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Reset the qubit
 */
void LightningQuantum::reset(Qubit q)
{
    rtd_qdevice->SetState({{0, 0}}, {q.value});    
}

//----------------------------------------------------------------------------//
/*!
 * Read the value of a result. This utilizes the new BufferManager.
 */
QState LightningQuantum::read_result(Result r)
{
    return results_[r.value]
}

//---------------------------------------------------------------------------//
/*!
 * Map a qubit to a result index.
 *
 * (TODO: find how to link the classical register to the quantum register in
 * qsim)
 */
void LightningQuantum::mz(Qubit q, Result r)
{ 
    QIREE_EXPECT(q.value < this->num_qubits());  // TODO: q must be in
                                                       // the set of qubits,
                                                       // e.g., what happens if
                                                       // q=5 and qubits are
                                                       // {2,3,4,5}, q is less
                                                       // than num_qubits but
                                                       // not it is in the set
                                                       // of qubits.
    // TODO: maybe not what we want long term
    QIREE_EXPECT(q.value == r.value);
    // Add measurement instruction
    results_[r.value] = rtd_qdevice->Measure(q.value, std::nullopt);
}

//---------------------------------------------------------------------------//
/*
 * Quantum Instruction Mapping
 */

// 1. Entangling gates
void LightningQuantum::cx(Qubit q1, Qubit q2)
{
    rtd_qdevice->NamedOperation(
        "CNOT", {}, {q1.value, q2.value});
}
void LightningQuantum::cnot(Qubit q1, Qubit q2)
{
    rtd_qdevice->NamedOperation(
        "CNOT", {}, {q1.value, q2.value});
}
void LightningQuantum::cz(Qubit q1, Qubit q2)
{
    rtd_qdevice->NamedOperation(
        "CZ", {}, {q1.value, q2.value});
}
// 2. Local gates
void LightningQuantum::h(Qubit q)
{
    rtd_qdevice->NamedOperation("Hadamard", {}, {q.value});
}
void LightningQuantum::s(Qubit q)
{
    rtd_qdevice->NamedOperation("S", {}, {q.value});
}
void LightningQuantum::t(Qubit q)
{
    rtd_qdevice->NamedOperation("T", {}, {q.value});
}
// 2.1 Pauli gates
void LightningQuantum::x(Qubit q)
{
    rtd_qdevice->NamedOperation("PauliX", {}, {q.value});
}
void LightningQuantum::y(Qubit q)
{
    rtd_qdevice->NamedOperation("PauliY", {}, {q.value});
}
void LightningQuantum::z(Qubit q)
{
    rtd_qdevice->NamedOperation("PauliZ", {}, {q.value});
}
// 2.2 rotation gates
void LightningQuantum::rx(double theta, Qubit q)
{
    rtd_qdevice->NamedOperation("RX", {theta}, {q.value});
}
void LightningQuantum::ry(double theta, Qubit q)
{
    rtd_qdevice->NamedOperation("RY", {theta}, {q.value});
}
void LightningQuantum::rz(double theta, Qubit q)
{
    rtd_qdevice->NamedOperation("RZ", {theta}, {q.value});
}

Qubit LightningQuantum::result_to_qubit(Result r)
{
    return result_to_qubit_[r.value];  
}

}  // namespace qiree
