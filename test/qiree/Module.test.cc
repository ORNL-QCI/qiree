//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Module.test.cc
//---------------------------------------------------------------------------//
#include "qiree/Module.hh"

#include "qiree_test.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//

class ModuleTest : public ::qiree::test::Test
{
  protected:
    void SetUp() override {}
};

//---------------------------------------------------------------------------//
TEST_F(ModuleTest, minimal)
{
    Module m(this->test_data_path("minimal.ll"), "main");
    EXPECT_TRUE(m);

    // Test attributes
    auto attrs = m.load_entry_point_attrs();
    EXPECT_EQ(0, attrs.required_num_qubits);
    EXPECT_EQ(0, attrs.required_num_results);
    EXPECT_EQ("", attrs.output_labeling_schema);
    EXPECT_EQ("custom", attrs.qir_profiles);

    // Test module flags
    auto flags = m.load_module_flags();
    EXPECT_EQ(1, flags.qir_major_version);
    EXPECT_EQ(0, flags.qir_minor_version);
    EXPECT_FALSE(flags.dynamic_qubit_management);
    EXPECT_FALSE(flags.dynamic_result_management);

    // Test move semantics
    Module other(std::move(m));
    EXPECT_FALSE(m);
    EXPECT_TRUE(other);
}

//---------------------------------------------------------------------------/
TEST_F(ModuleTest, multiple)
{
    Module m(this->test_data_path("multiple.ll"));
    EXPECT_TRUE(m);

    // Test attributes
    auto attrs = m.load_entry_point_attrs();
    EXPECT_EQ(2, attrs.required_num_qubits);
    EXPECT_EQ(2, attrs.required_num_results);
    EXPECT_EQ("", attrs.output_labeling_schema);
    EXPECT_EQ("", attrs.qir_profiles);

    // Test module flags
    auto flags = m.load_module_flags();
    EXPECT_EQ(0, flags.qir_major_version);
    EXPECT_EQ(0, flags.qir_minor_version);
    EXPECT_FALSE(flags.dynamic_qubit_management);
    EXPECT_FALSE(flags.dynamic_result_management);
}

//---------------------------------------------------------------------------/
TEST_F(ModuleTest, bell)
{
    Module m(this->test_data_path("bell.ll"));
    EXPECT_TRUE(m);

    // Test attributes
    auto attrs = m.load_entry_point_attrs();
    EXPECT_EQ(2, attrs.required_num_qubits);
    EXPECT_EQ(2, attrs.required_num_results);
    EXPECT_EQ("", attrs.output_labeling_schema);
    EXPECT_EQ("custom", attrs.qir_profiles);

    // Test module flags
    auto flags = m.load_module_flags();
    EXPECT_EQ(1, flags.qir_major_version);
    EXPECT_EQ(0, flags.qir_minor_version);
    EXPECT_FALSE(flags.dynamic_qubit_management);
    EXPECT_FALSE(flags.dynamic_result_management);
}

//---------------------------------------------------------------------------//
TEST_F(ModuleTest, several_gates)
{
    Module m(this->test_data_path("pyqir_several_gates.ll"));

    // Test attributes
    auto attrs = m.load_entry_point_attrs();
    EXPECT_EQ(4, attrs.required_num_qubits);
    EXPECT_EQ(4, attrs.required_num_results);
    EXPECT_EQ("", attrs.output_labeling_schema);
    EXPECT_EQ("custom", attrs.qir_profiles);

    // Test module flags
    auto flags = m.load_module_flags();
    EXPECT_EQ(1, flags.qir_major_version);
    EXPECT_EQ(0, flags.qir_minor_version);
    EXPECT_FALSE(flags.dynamic_qubit_management);
    EXPECT_FALSE(flags.dynamic_result_management);
}

//---------------------------------------------------------------------------//
TEST(ModuleTest, parse_ir_from_file) {

    // Helper function to read a file and return its contents as a string to be fed into from_bytes
    auto read_ll_file = [](const std::string& path) -> std::string {
        std::ifstream file(path);
        if (!file) throw std::runtime_error("Cannot open file: " + path);
        std::ostringstream buf;
        buf << file.rdbuf();
        return buf.str();
    };

    // Read the LLVM IR from a file and parse it
    std::string ir = read_ll_file("bell.ll");
    std::string_view ir_view(ir);

    // Expect no exceptions during parsing
    EXPECT_NO_THROW({
        Module m = Module::UPModule::from_bytes(ir_view);
    });
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
