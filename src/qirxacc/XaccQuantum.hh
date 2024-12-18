//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/XaccQuantum.hh
//---------------------------------------------------------------------------//
#pragma once

#include <initializer_list>
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "qiree/Macros.hh"
#include "qiree/QuantumNotImpl.hh"
#include "qiree/RuntimeInterface.hh"
#include "qiree/Types.hh"

namespace xacc
{
class AcceleratorBuffer;
class Accelerator;
class IRProvider;
class CompositeInstruction;
}  // namespace xacc

namespace qiree
{

//---------------------------------------------------------------------------//
/*!
 * Translate instructions from QIR to XACC and execute them on read.
 */
class XaccQuantum final : virtual public QuantumNotImpl
{
  public:
    // Call XACC initialize explicitly with args
    static void xacc_init(std::vector<std::string> args);

    // Construct with accelerator name and number of shots
    XaccQuantum(std::ostream& os,
                std::string const& accel_name,
                size_type shots);

    // Construct with simulator
    explicit XaccQuantum(std::ostream& os);

    // Call finalize when xacc is destroyed.
    ~XaccQuantum();

    QIREE_DELETE_COPY_MOVE(XaccQuantum);

    //!@{
    //! \name Accessors
    size_type num_results() const { return result_to_qubit_.size(); }
    size_type num_qubits() const { return num_qubits_; }
    //!@}

    //!@{
    //! \name Mutators
    // Update the XACC accelerator and shot count
    void set_accelerator_and_shots(
        std::string const& accel_name, size_type shots);
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
    //! \name Circuit construction
    void ccx(Qubit, Qubit, Qubit) final;
    void ccnot(Qubit, Qubit, Qubit);  // TODO: not in examples or qir runner
    void cnot(Qubit, Qubit) final;
    void cx(Qubit, Qubit) final;
    void cy(Qubit, Qubit) final;
    void cz(Qubit, Qubit) final;
    void h(Qubit) final;
    void reset(Qubit) final;
    void rx(double, Qubit) final;
    void ry(double, Qubit) final;
    void rz(double, Qubit) final;
    void rzz(double, Qubit, Qubit) final;
    void s(Qubit) final;
    void s_adj(Qubit) final;
    void swap(Qubit, Qubit) final;
    void t(Qubit) final;
    void t_adj(Qubit) final;
    void x(Qubit) final;
    void y(Qubit) final;
    void z(Qubit) final;
    //!@}

    //!@{
    //! \name Utilities for runtime
    // Get runtime qubit corresponding to a runtime result
    Qubit result_to_qubit(Result);

    // Return marginal statistics for a subset of qubits
    std::map<std::string, int>
    get_marginal_counts(std::vector<Qubit> const& qubits);

    // Run the circuit on the accelerator if we have not already. Returns true
    // if the circuit was executed.
    bool execute_if_needed();

    // Print the \c xacc::AcceleratorBuffer
    void print_accelbuf();
    //!@}

  private:
    //// TYPES ////
    enum class Endianness
    {
        little,
        big
    };

    //// DATA ////

    bool executed_{false};
    size_type num_qubits_{};
    std::vector<Qubit> result_to_qubit_;
    Endianness endian_;

    std::ostream& output_;
    std::shared_ptr<xacc::AcceleratorBuffer> buffer_;
    std::shared_ptr<xacc::Accelerator> accelerator_;
    std::shared_ptr<xacc::IRProvider> provider_;
    std::shared_ptr<xacc::CompositeInstruction> cur_circuit_;

    //// HELPER FUNCTIONS ////

    // Add an instruction with a single qubit
    template<class... Ts>
    void add_instruction(std::string s, Qubit q, Ts... args);

    // Add an instruction with multiple qubits
    template<class... Ts>
    void
    add_instruction(std::string s, std::initializer_list<Qubit> qs, Ts... args);

    // Add an instruction with multiple qubits to a particular XACC
    // CompositeInstruction
    template<class... Ts>
    void add_instruction_to(std::shared_ptr<xacc::CompositeInstruction> circuit,
                            std::string s,
                            std::initializer_list<Qubit> qs,
                            Ts... args);

    // Add an instruction with the control indices provided
    template<class... Ts>
    void add_ctrl_indices_instruction(std::string s,
                                      std::vector<int> ctrl_indices,
                                      Qubit q,
                                      Ts... args);

    // Add an instruction with the control indices provided as a list of QIR
    // pointers (for convenience)
    template<class... Ts>
    void add_ctrl_list_instruction(std::string s,
                                   std::initializer_list<Qubit> ctrl_list,
                                   Qubit q,
                                   Ts... args);
};

//---------------------------------------------------------------------------//
}  // namespace qiree
