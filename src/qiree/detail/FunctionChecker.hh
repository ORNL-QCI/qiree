//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/detail/FunctionChecker.hh
//---------------------------------------------------------------------------//
#pragma once

#include <llvm/IR/Function.h>

namespace qiree
{
namespace detail
{
//---------------------------------------------------------------------------//
static inline constexpr std::size_t unknown_arg_size = -1;

//---------------------------------------------------------------------------//
template<class F>
struct FuncTraits
{
    static constexpr std::size_t arg_size{unknown_arg_size};
};

//---------------------------------------------------------------------------//
template<class R>
struct FuncTraits<R (*)(void)>
{
    static constexpr std::size_t arg_size{0};
    using result_type = R;
};

template<class R, class A1>
struct FuncTraits<R (*)(A1)>
{
    static constexpr std::size_t arg_size{1};
    using result_type = R;
    using arg1_type = A1;
};

template<class R, class A1, class A2>
struct FuncTraits<R (*)(A1, A2)>
{
    static constexpr std::size_t arg_size{2};
    using result_type = R;
    using arg1_type = A1;
    using arg2_type = A2;
};

template<class R, class A1, class A2, class A3>
struct FuncTraits<R (*)(A1, A2, A3)>
{
    static constexpr std::size_t arg_size{3};
    using result_type = R;
    using arg1_type = A1;
    using arg2_type = A2;
    using arg3_type = A3;
};

//---------------------------------------------------------------------------//
/*!
 * Check compatibility between a C++ function and an LLVM function.
 */
class FunctionChecker
{
  public:
    // Construct with llvm function
    inline FunctionChecker(llvm::Function const& irfunc);

    // Check the given function's signature
    template<class F>
    inline void operator()(F* func) const;

  private:
    llvm::Function const& irfunc_;

    void check_impl(std::size_t num_args) const;
};

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
/*!
 * Construct with llvm function.
 */
FunctionChecker::FunctionChecker(llvm::Function const& irfunc)
    : irfunc_{irfunc}
{
    QIREE_VALIDATE(irfunc_.empty(),
                   << "could not bind to already-defined function '"
                   << irfunc_.getName().str() << "'");
}

//---------------------------------------------------------------------------//
/*!
 * Check the given function's signature against an LLVMIR function.
 */
template<class F>
void FunctionChecker::operator()(F*) const
{
    using TraitsT = FuncTraits<F*>;
    this->check_impl(TraitsT::arg_size);

    // TODO: traits classes for arguments (by value, reference, ...)
}

//---------------------------------------------------------------------------//
/*!
 * Perform non-templated safety checks on a function.
 */
void FunctionChecker::check_impl(std::size_t arg_size) const
{
    if (arg_size != unknown_arg_size)
    {
        QIREE_VALIDATE(arg_size == irfunc_.arg_size(),
                       << "incorrect QIR binding supplied: '"
                       << irfunc_.getName().str() << "' has "
                       << irfunc_.arg_size()
                       << " arguments but QIR-EE expected " << arg_size);
    }
}

//---------------------------------------------------------------------------//
}  // namespace detail
}  // namespace qiree
