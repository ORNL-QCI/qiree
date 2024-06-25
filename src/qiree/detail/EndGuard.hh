//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/detail/EndGuard.hh
//---------------------------------------------------------------------------//
#pragma once

namespace qiree
{
namespace detail
{
//---------------------------------------------------------------------------//
/*!
 * Call a function when this object is destroyed.
 */
template<class F>
class EndGuard
{
  public:
    //! Construct with functor
    EndGuard(F func) : func_{std::forward<F>(func)} {}

    //! Call functor on destruction
    ~EndGuard() { func_(); }

    QIREE_DELETE_COPY_MOVE(EndGuard);

  private:
    F func_;
};

//---------------------------------------------------------------------------//
// TEMPLATE DEDUCTION GUIDE
//---------------------------------------------------------------------------//
template<class F>
EndGuard(F&& func) -> EndGuard<F>;

//---------------------------------------------------------------------------//
}  // namespace detail
}  // namespace qiree
