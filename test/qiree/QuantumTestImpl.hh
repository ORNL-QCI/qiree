//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/QuantumTestImpl.hh
//---------------------------------------------------------------------------//
#pragma once

#include <sstream>

#include "qiree/QuantumInterface.hh"
#include "qiree/RuntimeInterface.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//
struct TestResult
{
    std::ostringstream commands;
};

//---------------------------------------------------------------------------//
/*!
 * Implementation of the quantum interface.
 */
class QuantumTestImpl final : public QuantumInterface
{
  public:
    // Construct with pointer to modifiable test result
    explicit QuantumTestImpl(TestResult* result);

    //! Prepare to build a quantum circuit for an entry point
    void set_up(EntryPointAttrs const&) final;

    //! Complete an execution
    void tear_down() final;

    //// Measurements ////

    // Measure the qubit and store in the result.
    void mz(Qubit, Result) final;

    // Read the value of a result.
    QState read_result(Result) final;

    //// Gates ////

    // Apply the H gate to the given qubit.
    void h(Qubit) final;

    // Apply the CNOT gate to the given qubits.
    void cnot(Qubit, Qubit) final;

    //// NOT IMPLEMENTED ////

    Result m(Qubit) override;
    Result measure(Array, Array) override;
    Result mresetz(Qubit) override;

    void ccx(Qubit, Qubit) override;
    void cx(Qubit, Qubit) override;
    void cy(Qubit, Qubit) override;
    void cz(Qubit, Qubit) override;
    void exp_adj(Array, double, Array) override;
    void exp(Array, double, Array) override;
    void exp(Array, Tuple) override;
    void exp_adj(Array, Tuple) override;
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

    void assertmeasurementprobability(
        Array, Array, Result, double, String, double) override {};
    void assertmeasurementprobability(Array, Tuple) override{};

  private:
    TestResult* tr_;
    unsigned int num_qubits_;
    unsigned int num_results_;
};

//---------------------------------------------------------------------------//
/*!
 * Implementation of the result output runtime interface.
 */
class ResultTestImpl final : public RuntimeInterface
{
  public:
    // Construct with pointer to modifiable test result
    explicit ResultTestImpl(TestResult* result);

    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) final;

    // Mark the following N results as being part of an array named tag
    void array_record_output(size_type, OptionalCString tag);

    // Mark the following N results as being part of a tuple named tag
    void tuple_record_output(size_type, OptionalCString tag);

    // Store one result
    void result_record_output(Result, OptionalCString tag) final;

  private:
    TestResult* tr_;
};

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
