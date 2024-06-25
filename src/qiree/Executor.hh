//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Executor.hh
//---------------------------------------------------------------------------//
#pragma once

#include <memory>
#include <string>

#include "Macros.hh"
#include "Types.hh"

namespace llvm
{
class Module;
class ExecutionEngine;
class Function;
}  // namespace llvm

namespace qiree
{
//---------------------------------------------------------------------------//
class Module;
class QuantumInterface;
class RuntimeInterface;

//---------------------------------------------------------------------------//
/*!
 * Set up and run an LLVM Execution Engine that wraps QIR.
 */
class Executor
{
  public:
    // Construct with a QIR input filename and function name
    explicit Executor(Module&& module);

    // Default destructor
    ~Executor();

    QIREE_DELETE_COPY_MOVE(Executor);

    // Execute with the given interface functions
    void operator()(QuantumInterface& qi, RuntimeInterface& ri) const;

  private:
    llvm::Function* entrypoint_{nullptr};
    llvm::Module* module_{nullptr};

    EntryPointAttrs entry_point_attrs_;
    ModuleFlags module_flags_;
    std::unique_ptr<llvm::ExecutionEngine> ee_;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
