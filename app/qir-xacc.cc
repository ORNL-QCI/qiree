//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file app/qir-xacc.cc
//---------------------------------------------------------------------------//
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <CLI/CLI.hpp>

#include "qiree_version.h"

#include "qiree/Executor.hh"
#include "qiree/Module.hh"
#include "qiree/QuantumNotImpl.hh"
#include "qirxacc/XaccDefaultRuntime.hh"
#include "qirxacc/XaccQuantum.hh"
#include "qirxacc/XaccTupleRuntime.hh"

using namespace std::string_view_literals;

int parse_input(int argc, char* argv[]);

//---------------------------------------------------------------------------//
/*!
 * Execute and run.
 */
int main(int argc, char* argv[])
{
    return parse_input(argc, argv);
}
