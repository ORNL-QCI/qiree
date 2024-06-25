//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/detail/GlobalMapper.hh
//---------------------------------------------------------------------------//
#pragma once

#include <type_traits>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#include "FunctionChecker.hh"
#include "qiree/Assert.hh"

namespace qiree
{
namespace detail
{
//---------------------------------------------------------------------------//
/*!
 * Map IR functions to compiled functions.
 */
class GlobalMapper
{
  public:
    // Construct with module and engine
    inline GlobalMapper(llvm::Module const& mod, llvm::ExecutionEngine* ee);

    // Map a symbol name to a compiled function pointer
    template<class F>
    inline void operator()(char const* name, F* func) const;

  private:
    llvm::Module const& mod_;
    llvm::ExecutionEngine* ee_;

    // Non-templated function checks
    inline void check_func(llvm::Function const& irfunc) const;
};

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
/*!
 * Construct with module and engine.
 */
GlobalMapper::GlobalMapper(llvm::Module const& mod, llvm::ExecutionEngine* ee)
    : mod_{mod}, ee_{ee}
{
    QIREE_EXPECT(ee_);
}

//---------------------------------------------------------------------------//
/*!
 * Map a symbol name to a compiled function pointer.
 */
template<class F>
void GlobalMapper::operator()(char const* name, F* func) const
{
    static_assert(std::is_function_v<F>, "not a function");

    llvm::Function* irfunc = mod_.getFunction(name);
    if (!irfunc)
    {
        // Function isn't available in the module (i.e. used by the current QIR
        // file)
        return;
    }

    // Throw an assertion if the function types don't match
    FunctionChecker{*irfunc}(func);

    return ee_->addGlobalMapping(irfunc, reinterpret_cast<void*>(func));
}

//---------------------------------------------------------------------------//
}  // namespace detail
}  // namespace qiree
