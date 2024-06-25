//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/MathInterface.hh
//---------------------------------------------------------------------------//
#pragma once

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Interface class for math operations in the \c qis (quantum) namespace.
 *
 * TODO: this is not yet implemented.
 *
 * \verbatim
double @__quantum__qis__arccos__body(double)
double @__quantum__qis__arcsin__body(double)
double @__quantum__qis__arctan__body(double)
double @__quantum__qis__arctan2__body(double)
double @__quantum__qis__cos__body(double)
double @__quantum__qis__cosh__body(double)
double @__quantum__qis__ieeeremainder__body(double, double)
double @__quantum__qis__infinity__body()
double @__quantum__qis__log__body(double)
double @__quantum__qis__nan__body()
double @__quantum__qis__sin__body(double)
double @__quantum__qis__sinh__body(double)
double @__quantum__qis__sqrt__body(double)
double @__quantum__qis__tan__body(double)
double @__quantum__qis__tanh__body(double)
i1 @__quantum__qis__isinf__body()
i1 @__quantum__qis__isnan__body()
i1 @__quantum__qis__isnegativeinfinity__body()

i64 @__quantum__qis__drawrandomint__body()
double @__quantum__qis__drawrandomdouble__body()
   \endverbatim
 */
class MathInterface
{
  public:
  protected:
    virtual ~MathInterface() = default;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
