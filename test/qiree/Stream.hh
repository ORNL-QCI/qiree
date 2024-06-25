//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Stream.hh
//---------------------------------------------------------------------------//
#pragma once

#include <ostream>

#include "qiree/Types.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
inline std::ostream& operator<<(std::ostream& os, Result r)
{
    os << "R{" << r.value << '}';
    return os;
}

//---------------------------------------------------------------------------//
inline std::ostream& operator<<(std::ostream& os, Qubit q)
{
    os << "Q{" << q.value << '}';
    return os;
}

//---------------------------------------------------------------------------//
inline std::ostream& operator<<(std::ostream& os, Tuple t)
{
    os << "T{" << t.value << '}';
    return os;
}

//---------------------------------------------------------------------------//
}  // namespace qiree
