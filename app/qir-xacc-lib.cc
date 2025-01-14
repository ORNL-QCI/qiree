//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qir-xacc/qir-xacc.cc
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

namespace qiree
{
namespace app
{
//---------------------------------------------------------------------------//
void run(std::string const& filename,
         std::string const& accel_name,
         int num_shots,
         bool print_accelbuf,
         bool group_tuples)
{
    // Load the input
    Executor execute{Module{filename}};

    // Set up XACC
    XaccQuantum xacc(std::cout, accel_name, num_shots);
    std::unique_ptr<RuntimeInterface> rt;
    if (group_tuples)
    {
        rt = std::make_unique<XaccTupleRuntime>(
            std::cout, xacc, print_accelbuf);
    }
    else
    {
        rt = std::make_unique<XaccDefaultRuntime>(
            std::cout, xacc, print_accelbuf);
    }

    // Run
    execute(xacc, *rt);
}

//---------------------------------------------------------------------------//
}  // namespace app
}  // namespace qiree

//---------------------------------------------------------------------------//
/*!
 * Execute and run.
 */
int parse_input(int argc, char* argv[])
{
    int num_shots{1024};
    std::string accel_name;
    std::string filename;
    bool no_print_accelbuf{false};
    bool group_tuples{false};

    CLI::App app;
    auto* filename_opt
        = app.add_option("--input,-i,input", filename, "QIR input file");
    filename_opt->required();
    auto* accel_opt
        = app.add_option("-a,--accelerator", accel_name, "Accelerator name");
    accel_opt->required();
    auto* nshot_opt
        = app.add_option("-s,--shots", num_shots, "Number of shots");
    nshot_opt->capture_default_str();
    app.add_flag("--no-print-xacc-accelbuf",
                 no_print_accelbuf,
                 "Do not print XACC AcceleratorBuffer");
    app.add_flag("--group-tuples",
                 group_tuples,
                 "Print per-tuple/per-array measurement statistics rather "
                 "than per-qubit");

    std::cout << argc << "argv" << argv << std::endl;

    CLI11_PARSE(app, argc, argv);

    qiree::app::run(
        filename, accel_name, num_shots, !no_print_accelbuf, group_tuples);

    return EXIT_SUCCESS;
}

extern "C" void parse_input_c(int argc, char* argv[]){
        parse_input(argc, argv);
}
