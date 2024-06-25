//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file Test.cc
//---------------------------------------------------------------------------//
#include "Test.hh"

#include "qiree_test_config.h"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//
/*!
 * Get the full path to a data file in the test directory.
 */
std::string Test::test_data_path(std::string const& filename)
{
    std::string result = qiree_source_dir;
    result += "/test/data/";
    result += filename;
    return result;
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
