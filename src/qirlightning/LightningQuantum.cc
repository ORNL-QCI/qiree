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
#include <dlfcn.h>
#include <random>

#include "qiree/Assert.hh"

extern "C" Catalyst::Runtime::QuantumDevice*
GenericDeviceFactory(char const* kwargs);
namespace qiree
{
using namespace Catalyst::Runtime;

//---------------------------------------------------------------------------//
/*!
 * Initialize the Lightning simulator
 */
LightningQuantum::LightningQuantum(std::ostream& os, unsigned long int seed) : output_(os), seed_(seed)
{
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
    results_.resize(attrs.required_num_results);


    // We load the library every time because we currently have an issue 
    // with releasing qubits in Catalyst.
    // Once that is fixed, this can go to the constructor to execute once
    std::string rtd_lib = RTDLIB;
    std::string rtd_device = RTDDEVICE;
    std::string kwargs = {};
    auto rtld_flags = RTLD_LAZY | RTLD_NODELETE;
    rtd_dylib_handler = dlopen(rtd_lib.c_str(), rtld_flags);

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

    rtd_qdevice->AllocateQubits(num_qubits_);
}

//---------------------------------------------------------------------------//
/*!
 * Complete an execution
 */
void LightningQuantum::tear_down()
{
    // This should go to the destructor once we fix the issue with releasing qubits
    if (rtd_dylib_handler)
    {
        dlclose(rtd_dylib_handler);
    };
  
}

//---------------------------------------------------------------------------//
/*!
 * Reset the qubit
 */
void LightningQuantum::reset(Qubit q)
{
    std::vector<int8_t> data = {0}; 
    DataView<int8_t, 1> state(data);
    std::vector<QubitIdType> wires = {static_cast<intptr_t>(q.value)};    rtd_qdevice->SetBasisState(state, wires);    
}

//----------------------------------------------------------------------------//
/*!
 * Read the value of a result. 
 */
QState LightningQuantum::read_result(Result r)
{
    return this->get_result(r);
}

//---------------------------------------------------------------------------//
/*!
 * Map a qubit to a result index.
 */
void LightningQuantum::mz(Qubit q, Result r)
{ 
    QIREE_EXPECT(q.value < this->num_qubits());  
    QIREE_EXPECT(r.value < this->num_results());
    std::mt19937 gen(seed_);
    seed_++;
    rtd_qdevice->SetDevicePRNG(&gen);
    auto result = rtd_qdevice->Measure(static_cast<intptr_t>(q.value), std::nullopt);
    results_[r.value] = *result;
}

//---------------------------------------------------------------------------//
/*
 * Quantum Instruction Mapping
 */

// 1. Entangling gates
void LightningQuantum::cx(Qubit q1, Qubit q2)
{
    rtd_qdevice->NamedOperation(
        "CNOT", {}, {static_cast<intptr_t>(q1.value), static_cast<intptr_t>(q2.value)});
}
void LightningQuantum::cnot(Qubit q1, Qubit q2)
{
    rtd_qdevice->NamedOperation(
        "CNOT", {}, {static_cast<intptr_t>(q1.value), static_cast<intptr_t>(q2.value)});
}
void LightningQuantum::cz(Qubit q1, Qubit q2)
{
    rtd_qdevice->NamedOperation(
        "CZ", {}, {static_cast<intptr_t>(q1.value), static_cast<intptr_t>(q2.value)});
}
// 2. Local gates
void LightningQuantum::h(Qubit q)
{
    rtd_qdevice->NamedOperation("Hadamard", {}, {static_cast<intptr_t>(q.value)});
}
void LightningQuantum::s(Qubit q)
{
    rtd_qdevice->NamedOperation("S", {}, {static_cast<intptr_t>(q.value)});
}
void LightningQuantum::t(Qubit q)
{
    rtd_qdevice->NamedOperation("T", {}, {static_cast<intptr_t>(q.value)});
}
// 2.1 Pauli gates
void LightningQuantum::x(Qubit q)
{
    rtd_qdevice->NamedOperation("PauliX", {}, {static_cast<intptr_t>(q.value)});
}
void LightningQuantum::y(Qubit q)
{
    rtd_qdevice->NamedOperation("PauliY", {}, {static_cast<intptr_t>(q.value)});
}
void LightningQuantum::z(Qubit q)
{
    rtd_qdevice->NamedOperation("PauliZ", {}, {static_cast<intptr_t>(q.value)});
}
// 2.2 rotation gates
void LightningQuantum::rx(double theta, Qubit q)
{
    rtd_qdevice->NamedOperation("RX", {theta}, {static_cast<intptr_t>(q.value)});
}
void LightningQuantum::ry(double theta, Qubit q)
{
    rtd_qdevice->NamedOperation("RY", {theta}, {static_cast<intptr_t>(q.value)});
}
void LightningQuantum::rz(double theta, Qubit q)
{
    rtd_qdevice->NamedOperation("RZ", {theta}, {static_cast<intptr_t>(q.value)});
}

}  // namespace qiree
