//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Module.hh
//---------------------------------------------------------------------------//
#pragma once

#include <memory>

#include "Types.hh"

namespace llvm
{
class Module;
class Function;
}  // namespace llvm

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Load a QIR LLVM module.
 */
class Module
{
  public:
    //!@{
    //! \name Type aliases
    using UPModule = std::unique_ptr<llvm::Module>;
    //!@}

  public:
    // Default empty constructor
    Module();
    // Externally defined defaults
    ~Module();
    Module(Module&&);
    Module& operator=(Module&&);
    // Prevent copying
    Module(Module const&) = delete;
    Module& operator=(Module const&) = delete;

    // Construct from an externally created LLVM module
    explicit Module(UPModule&& module);

    // Construct with an LLVM IR file (bitcode or disassembled)
    explicit Module(std::string const& filename);

    // Construct with an LLVM IR file (bitcode or disassembled) and entry point
    Module(std::string const& filename, std::string const& entrypoint);

    // Process entry point attributes
    EntryPointAttrs load_entry_point_attrs() const;

    // Translate module attributes into flags
    ModuleFlags load_module_flags() const;

    //! True if the module has been constructed (and not moved)
    explicit operator bool() const { return static_cast<bool>(module_); }

  private:
    std::unique_ptr<llvm::Module> module_;
    llvm::Function* entrypoint_{nullptr};

    // Make Executor a friend so it can take ownership of the pointer
    friend class Executor;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
