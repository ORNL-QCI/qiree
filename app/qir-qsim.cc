//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file app/qir-qsim.cc
//---------------------------------------------------------------------------//
#include <cstdlib>
#include <iostream>
#include <string>
#include <CLI/CLI.hpp>

#include "qiree/Executor.hh"
#include "qiree/Module.hh"
#include "qiree/ResultDistribution.hh"
#include "qirqsim/QsimDefaultRuntime.hh"
#include "qirqsim/QsimQuantum.hh"

using namespace std::string_view_literals;

int parse_input(int argc, char* argv[]);
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Execute and run.
 */
int main(int argc, char* argv[])
{
    return parse_input(argc, argv);
}
