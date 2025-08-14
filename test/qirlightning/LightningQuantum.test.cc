//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirlightning/LightningQuantum.test.cc
//---------------------------------------------------------------------------//
#include "qirlightning/LightningQuantum.hh"

#include <regex>

#include "qiree/Types.hh"
#include "qiree_test.hh"
#include "qirlightning/LightningRuntime.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//

class LightningQuantumTest : public ::qiree::test::Test
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

TEST_F(LightningQuantumTest, sim_dynamicbv)
{
    using Q = Qubit;
    using R = Result;

    std::ostringstream os;
    os << '\n';

    // Create a simulator that will write to the string stream
    LightningQuantum lightning_sim{os, 0};
    LightningRuntime lightning_rt{os, lightning_sim};
    // Call functions in the same sequence that dynamicbv.ll would
    lightning_sim.set_up([] {
        EntryPointAttrs attrs;
        attrs.required_num_qubits = 2;
        attrs.required_num_results = 2;
        return attrs;
    }());
    ASSERT_EQ(2, lightning_sim.num_qubits());
    ASSERT_EQ(2, lightning_sim.num_results());

    lightning_sim.h(Q{0});
    lightning_sim.x(Q{1});
    lightning_sim.h(Q{1});
    lightning_sim.cnot(Q{0}, Q{1});
    lightning_sim.h(Q{0});
    lightning_sim.mz(Q{0}, R{0});
    lightning_sim.read_result(R{0});
    lightning_sim.mz(Q{1}, R{1});
    lightning_sim.read_result(R{1});
    lightning_rt.array_record_output(2, "");
    lightning_rt.result_record_output(R{0}, "");
    lightning_rt.result_record_output(R{1}, "");
    EXPECT_EQ(QState::one, lightning_sim.read_result(R{0}));

    lightning_sim.tear_down();
}

TEST_F(LightningQuantumTest, result_order)
{
    using Q = Qubit;
    using R = Result;

    std::ostringstream os;
    os << '\n';

    // Create a simulator that will write to the string stream
    LightningQuantum qis{os, 0};
    LightningRuntime rt{os, qis};

    // Call functions in the same sequence that dynamicbv.ll would
    qis.set_up([] {
        EntryPointAttrs attrs;
        attrs.required_num_qubits = 4;
        attrs.required_num_results = 3;
        return attrs;
    }());
    qis.mz(Q{0}, R{2});
    qis.mz(Q{1}, R{1});
    qis.mz(Q{2}, R{0});
    std::vector<bool> expected;
    expected.push_back(static_cast<bool>(qis.read_result(R{2})));
    expected.push_back(static_cast<bool>(qis.read_result(R{0})));
    expected.push_back(static_cast<bool>(qis.read_result(R{1})));
    // So the internal result "buffer" is now {true, false, true}
    rt.array_record_output(3, "array");
    rt.result_record_output(R{2}, "foo");  // pushes true
    rt.result_record_output(R{0}, "bar");  // pushes true
    rt.result_record_output(R{1}, "baz");  // pushes false

    auto const& result = rt.result();
    EXPECT_EQ("array", result.container_label());
    EXPECT_EQ(expected, result.bits());
    EXPECT_EQ((std::vector<std::string>{"foo", "bar", "baz"}),
              result.entry_labels());

    qis.tear_down();
}
//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
