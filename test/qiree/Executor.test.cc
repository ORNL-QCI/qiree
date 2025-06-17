//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Executor.test.cc
//---------------------------------------------------------------------------//
#include "qiree/Executor.hh"

#include "QuantumTestImpl.hh"
#include "qiree/Assert.hh"
#include "qiree/Module.hh"
#include "qiree_test.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//

class ExecutorTest : public ::qiree::test::Test
{
  protected:
    void SetUp() override {}

    TestResult run(std::string const& filename);
    TestResult run(std::string const& filename, std::string const& entry);

  private:
    TestResult run_impl(Module&& m);
};

//---------------------------------------------------------------------------//
TestResult ExecutorTest::run(std::string const& filename)
{
    return this->run_impl(Module(this->test_data_path(filename)));
}

//---------------------------------------------------------------------------//
TestResult
ExecutorTest::run(std::string const& filename, std::string const& entry)
{
    return this->run_impl(Module(this->test_data_path(filename), entry));
}

//---------------------------------------------------------------------------//
TestResult ExecutorTest::run_impl(Module&& m)
{
    QIREE_EXPECT(m);
    Executor execute(std::move(m));

    // Run with the test interface
    TestResult tr;
    QuantumTestImpl quantum_impl(&tr);
    ResultTestImpl result_impl(&tr);

    execute(quantum_impl, result_impl);
    return tr;
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, minimal)
{
    auto result = this->run("minimal.ll");
    EXPECT_EQ(R"(
set_up(q=0, r=0)
tear_down
)",
              result.commands.str());
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, bit_flip_error_correction)
{
    auto result = this->run("unitaryhack2025/bit_flip_error_correction.ll");
    EXPECT_EQ(R"(
set_up(q=5, r=3)
cnot(Q{0}, Q{1})
cnot(Q{0}, Q{2})
TODO: x.body
TODO: cx.body
TODO: cx.body
mz(Q{3},R{0})
array_record_output(3)
result_record_output(R{0})
read_result(R{0})
TODO: cx.body
TODO: cx.body
mz(Q{4},R{1})
result_record_output(R{1})
read_result(R{1})
mz(Q{0},R{2})
result_record_output(R{2})
tear_down
)",
              result.commands.str());
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, kitaev_phase_estimation)
{
    auto result = this->run("unitaryhack2025/kitaev_phase_estimation.ll");
    EXPECT_EQ(R"(
set_up(q=2, r=2)
TODO: x.body
h(Q{0})
TODO: cz.body
h(Q{0})
mz(Q{0},R{0})
array_record_output(2)
result_record_output(R{0})
read_result(R{0})
TODO: reset.body
h(Q{0})
TODO: cz.body
TODO: cz.body
h(Q{0})
mz(Q{0},R{1})
result_record_output(R{1})
tear_down
)",
              result.commands.str());
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, bell)
{
    auto result = this->run("bell.ll");
    EXPECT_EQ(R"(
set_up(q=2, r=2)
h(Q{0})
cnot(Q{0}, Q{1})
mz(Q{0},R{0})
mz(Q{1},R{1})
array_record_output(2)
result_record_output(R{0})
result_record_output(R{1})
tear_down
)",
              result.commands.str());
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, bell_ccx)
{
    auto result = this->run("bell_ccx.ll");
    EXPECT_EQ(R"(
set_up(q=3, r=3)
h(Q{0})
TODO: x.body
ccx(Q{0}, Q{1}, Q{2})
mz(Q{0},R{0})
mz(Q{1},R{1})
mz(Q{2},R{2})
array_record_output(3)
result_record_output(R{0})
result_record_output(R{1})
result_record_output(R{2})
tear_down
)",
              result.commands.str());
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, rotation)
{
    auto result = this->run("rotation.ll");
    EXPECT_EQ(R"(
set_up(q=1, r=1)
h(Q{0})
rx(0.523599, Q{0})
mz(Q{0},R{0})
array_record_output(1)
result_record_output(R{0})
tear_down
)",
              result.commands.str())
        << result.commands.str();
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, several_gates)
{
    auto result = this->run("pyqir_several_gates.ll");
    EXPECT_EQ(R"(
set_up(q=4, r=4)
h(Q{0})
cnot(Q{0}, Q{1})
TODO: cz.body
rx(0.523599, Q{1})
ry(1.047, Q{2})
rz(1.571, Q{3})
s(Q{0})
TODO: s.adj
TODO: t.body
TODO: t.adj
TODO: x.body
TODO: y.body
TODO: z.body
mz(Q{0},R{0})
mz(Q{1},R{1})
mz(Q{2},R{2})
mz(Q{3},R{3})
array_record_output(4)
result_record_output(R{0})
result_record_output(R{1})
result_record_output(R{2})
result_record_output(R{3})
tear_down
)",
              result.commands.str());
    // cout << result.commands.str();
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, loop)
{
    // NOTE: attribute is misnamed "EntryPoint"
    auto result = this->run("loop.ll", "main");
    EXPECT_EQ(R"(
set_up(q=1, r=1)
h(Q{0})
h(Q{0})
h(Q{0})
h(Q{0})
h(Q{0})
mz(Q{0},R{0})
array_record_output(1)
result_record_output(R{0})
tear_down
)",
              result.commands.str());
}

//---------------------------------------------------------------------------//
TEST_F(ExecutorTest, teleport)
{
    auto result = this->run("teleport.ll");
    EXPECT_EQ(R"(
set_up(q=3, r=3)
h(Q{1})
cnot(Q{1}, Q{2})
cnot(Q{0}, Q{1})
h(Q{0})
mz(Q{0},R{0})
TODO: reset.body
read_result(R{0})
mz(Q{1},R{1})
TODO: reset.body
read_result(R{1})
mz(Q{2},R{2})
array_record_output(3)
result_record_output(R{0})
result_record_output(R{1})
result_record_output(R{2})
tear_down
)",
              result.commands.str());
    // cout << result.commands.str();
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
