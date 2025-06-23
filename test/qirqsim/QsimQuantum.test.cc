//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/QsimQuantum.test.cc
//---------------------------------------------------------------------------//
#include "qirqsim/QsimQuantum.hh"

#include <regex>

#include "qiree/Types.hh"
#include "qiree_test.hh"
#include "qirqsim/QsimRuntime.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//

class QsimQuantumTest : public ::qiree::test::Test
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

TEST_F(QsimQuantumTest, sim_dynamicbv)
{
    using Q = Qubit;
    using R = Result;

    std::ostringstream os;
    os << '\n';

    // Create a simulator that will write to the string stream
    QsimQuantum qsim_sim{os, 0};
    QsimRuntime qsim_rt{os, qsim_sim};

    // Call functions in the same sequence that dynamicbv.ll would
    qsim_sim.set_up([] {
        EntryPointAttrs attrs;
        attrs.required_num_qubits = 2;
        attrs.required_num_results = 2;
        return attrs;
    }());
    ASSERT_EQ(2, qsim_sim.num_qubits());
    ASSERT_EQ(2, qsim_sim.num_results());

    qsim_sim.h(Q{0});
    qsim_sim.x(Q{1});
    qsim_sim.h(Q{1});
    qsim_sim.cnot(Q{0}, Q{1});
    qsim_sim.h(Q{0});
    qsim_sim.mz(Q{0}, R{0});
    qsim_sim.read_result(R{0});
    qsim_sim.mz(Q{1}, R{1});
    qsim_sim.read_result(R{1});
    qsim_rt.array_record_output(2, "");
    qsim_rt.result_record_output(R{0}, "");
    qsim_rt.result_record_output(R{1}, "");
    EXPECT_EQ(QState::one, qsim_sim.get_result(R{0}));
    EXPECT_EQ(QState::one, qsim_sim.get_result(R{1}));

    qsim_sim.h(Q{0});
    qsim_sim.x(Q{1});
    qsim_sim.h(Q{1});
    qsim_sim.mz(Q{0}, R{0});
    qsim_sim.read_result(R{0});
    qsim_sim.mz(Q{1}, R{1});
    qsim_sim.read_result(R{1});
    qsim_rt.array_record_output(2, "");
    qsim_rt.result_record_output(R{0}, "");
    qsim_rt.result_record_output(R{1}, "");
    EXPECT_EQ(QState::zero, qsim_sim.get_result(R{0}));
    EXPECT_EQ(QState::zero, qsim_sim.get_result(R{1}));

    qsim_sim.h(Q{0});
    qsim_sim.x(Q{1});
    qsim_sim.h(Q{1});
    qsim_sim.cnot(Q{0}, Q{1});
    qsim_sim.h(Q{0});
    qsim_sim.mz(Q{0}, R{0});
    qsim_sim.read_result(R{0});
    qsim_sim.mz(Q{1}, R{1});
    qsim_sim.read_result(R{1});
    qsim_rt.array_record_output(2, "");
    qsim_rt.result_record_output(R{0}, "");
    qsim_rt.result_record_output(R{1}, "");
    EXPECT_EQ(QState::one, qsim_sim.get_result(R{0}));
    EXPECT_EQ(QState::zero, qsim_sim.get_result(R{1}));

    qsim_sim.tear_down();
}

TEST_F(QsimQuantumTest, result_order)
{
    using Q = Qubit;
    using R = Result;

    std::ostringstream os;
    os << '\n';

    // Create a simulator that will write to the string stream
    QsimQuantum qis{os, 0};
    QsimRuntime rt{os, qis};

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
    expected.push_back(static_cast<bool>(qis.get_result(R{2})));
    expected.push_back(static_cast<bool>(qis.get_result(R{0})));
    expected.push_back(static_cast<bool>(qis.get_result(R{1})));
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
