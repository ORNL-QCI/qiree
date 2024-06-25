//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file Test.hh
//---------------------------------------------------------------------------//
#pragma once

#include <string>
#include <gtest/gtest.h>

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//
/*!
 * QIR-EE Google testing harness.
 */
class Test : public ::testing::Test
{
  public:
    // Get the full path to a data file in the test directory
    static std::string test_data_path(std::string const& filename);
};

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
