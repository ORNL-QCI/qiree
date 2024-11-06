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

#include "qiree/Executor.hh"
#include "qiree/Module.hh"
#include "qiree/QuantumNotImpl.hh"

#include "qirqsim/qsimDefaultRuntime.hh"
#include "qirqsim/qsimQuantum.hh"
#include "qirqsim/qsimTupleRuntime.hh"

using namespace std::string_view_literals;

namespace qiree
{
namespace app
{
void run(std::string const& filename,
         int num_shots)
         // bool group_tuples = false)
{
    // Load the input
    Executor execute{Module{filename}};
    
    // Set up qsim
    qsimQuantum sim(std::cout, num_shots);
    
    // Collect the statistics 
    std::unique_ptr<RuntimeInterface> rt;
    //if (group_tuples){
    //    rt = std::make_unique<qsimTupleRuntime>(
    //        std::cout, sim);
    //} else {
        rt = std::make_unique<qsimDefaultRuntime>(
            std::cout, sim);
    //}

    // Run several time = shots (default 1)
    for (int i = 0; i < num_shots; i++){    
        execute(sim, *rt);
    }
}

//---------------------------------------------------------------------------//
}  // namespace app
}  // namespace qiree

//---------------------------------------------------------------------------//
/*!
 * Execute and run.
 */
int main(int argc, char* argv[])
{
    int num_shots{1};
    std::string filename;
    //bool group_tuples{false};
 
    CLI::App app;

    auto* filename_opt
        = app.add_option("--input,-i,input", filename, "QIR input file");
    filename_opt->required();

    auto* nshot_opt
        = app.add_option("-s,--shots", num_shots, "Number of shots");
    nshot_opt->capture_default_str();
    
    //app.add_flag("--group-tuples,!--no-group-tuples",
    //            group_tuples,
    //            "Print per-tuple measurement statistics rather than "
    //            "per-qubit");
    
    CLI11_PARSE(app, argc, argv);

    //qiree::app::run(filename, num_shots, group_tuples);
    qiree::app::run(filename, num_shots);
        
    return EXIT_SUCCESS;
}
