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
#include "qirqsim/QsimDefaultRuntime.hh"

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
    QsimDefaultRuntime qsim_rt{os, qsim_sim};

    // Call functions in the same sequence that dynamicbv.ll would
    qsim_sim.set_up([] {
        EntryPointAttrs attrs;
        attrs.required_num_qubits = 2;
        attrs.required_num_results = 2;
        return attrs;
    }());
    qsim_sim.h(Q{0});
    qsim_sim.x(Q{1});
    qsim_sim.h(Q{1});
    qsim_sim.cnot(Q{0},Q{1});
    qsim_sim.h(Q{0});
    qsim_sim.mz(Q{0}, R{0});
    qsim_sim.read_result(R{0});
    qsim_sim.mz(Q{1}, R{1});
    qsim_sim.read_result(R{1});
    qsim_rt.array_record_output(2,"");
    qsim_rt.result_record_output(R{0},"");
    qsim_rt.result_record_output(R{1},"");
    qsim_sim.h(Q{0});
    qsim_sim.x(Q{1});
    qsim_sim.h(Q{1});
    qsim_sim.mz(Q{0}, R{0});
    qsim_sim.read_result(R{0});
    qsim_sim.mz(Q{1}, R{1});
    qsim_sim.read_result(R{1});
    qsim_rt.array_record_output(2,"");
    qsim_rt.result_record_output(R{0},"");
    qsim_rt.result_record_output(R{1},"");
    qsim_sim.h(Q{0});
    qsim_sim.x(Q{1});
    qsim_sim.h(Q{1});
    qsim_sim.cnot(Q{0},Q{1});
    qsim_sim.h(Q{0});
    qsim_sim.mz(Q{0}, R{0});
    qsim_sim.read_result(R{0});
    qsim_sim.mz(Q{1}, R{1});
    qsim_sim.read_result(R{1});
    qsim_rt.array_record_output(2,"");
    qsim_rt.result_record_output(R{0},"");
    qsim_rt.result_record_output(R{1},"");
    qsim_sim.tear_down();

    ASSERT_EQ(2, qsim_sim.num_qubits());
    EXPECT_EQ(1, qsim_sim.manager.getBufferValue("q0", "0").value());
    EXPECT_EQ(2, qsim_sim.manager.getBufferValue("q0", "1").value());
    EXPECT_EQ(2, qsim_sim.manager.getBufferValue("q1", "0").value());
    EXPECT_EQ(1, qsim_sim.manager.getBufferValue("q1", "1").value());
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
