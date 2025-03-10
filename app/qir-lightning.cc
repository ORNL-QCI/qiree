//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file app/qir-lightning.cc
//---------------------------------------------------------------------------//
#include <cstdlib>
#include <iostream>
#include <string>
#include <CLI/CLI.hpp>

#include "qiree/Executor.hh"
#include "qiree/Module.hh"
#include "qiree/ResultDistribution.hh"
#include "qirlightning/LightningDefaultRuntime.hh"
#include "qirlightning/LightningQuantum.hh"

using namespace std::string_view_literals;

namespace qiree
{
namespace app
{
//---------------------------------------------------------------------------//
void run(std::string const& filename, int num_shots)
{
    // Load the input
    Executor execute{Module{filename}};

    // Set up qsim
    LightningQuantum sim(std::cout, 0);
    LightningDefaultRuntime rt(std::cout, sim);
    ResultDistribution distribution;

    // Run several time = shots (default 1)
    for (int i = 0; i < num_shots; i++)
    {
        execute(sim, rt);
        distribution.accumulate(rt.result());
    }

    std::cout << distribution.to_json() << std::endl;
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

    CLI::App app;

    auto* filename_opt
        = app.add_option("--input,-i,input", filename, "QIR input file");
    filename_opt->required();

    auto* nshot_opt
        = app.add_option("-s,--shots", num_shots, "Number of shots");
    nshot_opt->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    qiree::app::run(filename, num_shots);

    return EXIT_SUCCESS;
}
