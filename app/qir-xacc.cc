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

#include "qiree_version.h"

#include "qiree/Executor.hh"
#include "qiree/Module.hh"
#include "qiree/QuantumNotImpl.hh"
#include "qirxacc/XaccQuantum.hh"

using namespace std::string_view_literals;

namespace qiree
{
namespace app
{
//---------------------------------------------------------------------------//
void run(std::string const& filename,
         std::string const& accel_name,
         int num_shots)
{
    // Load the input
    Executor execute{Module{filename}};

    // Set up XACC
    XaccQuantum xacc(std::cout, accel_name, num_shots);

    // Run
    execute(xacc, xacc);
}

//---------------------------------------------------------------------------//
void print_usage(std::string_view exec_name)
{
    // clang-format off
    std::cerr << "usage: " << exec_name << " input.ll accelerator num_shots\n"
                 "       " << exec_name << " [--help|-h]\n"
                 "       " << exec_name << " --version\n";
    // clang-format on
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
    using std::cerr;
    using std::cout;
    using std::endl;

    // Process input arguments
    int return_code = EXIT_SUCCESS;

    if (argc == 2)
    {
        std::string_view flag{argv[1]};
        if (flag == "--help"sv || flag == "-h"sv)
        {
            qiree::app::print_usage(argv[0]);
        }
        else if (flag == "--version"sv || flag == "-v"sv)
        {
            std::cout << qiree_version << std::endl;
        }
    }
    else if (argc == 4)
    {
        std::string filename{argv[1]};
        try
        {
            qiree::app::run(filename, argv[2], std::atoi(argv[3]));
        }
        catch (std::exception const& e)
        {
            std::cerr << "fatal: while running input at " << filename << ":\n"
                      << e.what() << std::endl;
            return_code = EXIT_FAILURE;
        }
    }
    else
    {
        qiree::app::print_usage(argv[0]);
        return_code = EXIT_FAILURE;
    }

    return return_code;
}
