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
#include "qirqsim/QsimDefaultRuntime.hh"
#include "qirqsim/QsimQuantum.hh"

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
    QsimQuantum sim(std::cout, 0);

    // Collect the statistics
    std::unique_ptr<RuntimeInterface> rt;
    rt = std::make_unique<QsimDefaultRuntime>(std::cout, sim);

    // Run several time = shots (default 1)
    for (int i = 0; i < num_shots; i++)
    {
        execute(sim, *rt);
    }

    std::cout << std::endl;
    std::cout << "Measurement output:" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "Number of shots: " << num_shots << std::endl;
    std::cout << "Number of qubits: " << sim.num_qubits() << std::endl;

    for(int q_index = 0; q_index < sim.num_qubits(); q_index++){
        int value_0 = 0;
        int value_1 = 0;
        if (auto value = sim.manager.getBufferValue("q"+std::to_string(q_index), "0"); value.has_value()){ value_0 = value.value();}
        if (auto value = sim.manager.getBufferValue("q"+std::to_string(q_index), "1"); value.has_value()){ value_1 = value.value();}
        std::cout << "q" << q_index << " {0: " << value_0 << "," << " 1: " << value_1 << "}\n";
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
