//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/XaccQuantum.test.cc
//---------------------------------------------------------------------------//
#include "qirxacc/XaccQuantum.hh"

#include <regex>

#include "qiree/Types.hh"
#include "qiree_test.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//
constexpr double pi = 3.141592653589793;

class XaccQuantumTest : public ::qiree::test::Test
{
  protected:
    void SetUp() override {}

    static std::string clean_output(std::string&& s)
    {
        std::string result = std::move(s);
        static std::regex const subs_ptr("0x[0-9a-f]+");
        result = std::regex_replace(result, subs_ptr, "0x0");
        return result;
    }
};

// Use "death test" to list compilers and accelerators since xacc calls
// std::exit. To enable, remove the following line or run
// ./test/qirxacc_XaccQuantumTest \
//   '--gtest_filter=*DeathTest*' \
//   '--gtest_also_run_disabled_tests
#define XaccQuantumDeathTest DISABLED_XaccQuantumDeathTest
using XaccQuantumDeathTest = XaccQuantumTest;

TEST_F(XaccQuantumDeathTest, list_compilers)
{
    EXPECT_EXIT(XaccQuantum::xacc_init({"--list-compilers"}),
                testing::ExitedWithCode(0),
                "" /* no stderr output; it uses cout */);
}

TEST_F(XaccQuantumDeathTest, list_accelerators)
{
    EXPECT_EXIT(XaccQuantum::xacc_init({"--list-accelerators"}),
                testing::ExitedWithCode(0),
                "" /* no stderr output; it uses cout */);
}

TEST_F(XaccQuantumTest, sim_rotation)
{
    using Q = Qubit;
    using R = Result;

    std::ostringstream os;
    os << '\n';

    // Create a simulator that will write to the string stream
    XaccQuantum xacc_sim{os};

    // Call functions in the same sequence that rotation.ll would
    xacc_sim.set_up([] {
        EntryPointAttrs attrs;
        attrs.required_num_qubits = 1;
        attrs.required_num_results = 1;
        return attrs;
    }());
    xacc_sim.h(Q{0});
    xacc_sim.rx(pi / 6, Q{0});
    xacc_sim.mz(Q{0}, R{0});
    xacc_sim.array_record_output(1, nullptr);
    xacc_sim.result_record_output(R{0}, "tag");
    xacc_sim.tear_down();

    auto result = clean_output(os.str());
    EXPECT_EQ(R"(
{
    "AcceleratorBuffer": {
        "name": "qreg_0x0",
        "size": 1,
        "Information": {},
        "Measurements": {
            "": 1
        }
    }
}
qubit 0 experiment tag: {0: 0, 1: 0}
)",
              result)
        << result;
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
