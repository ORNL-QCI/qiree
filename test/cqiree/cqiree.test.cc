//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/cqiree.test.cc
//---------------------------------------------------------------------------//
#include "cqiree/CQiree.h"

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <dlfcn.h>

#include "qiree_targets.h"

#include "qiree/Assert.hh"
#include "qiree_test.hh"

// Declare function pointer types, e.g. qiree_create_t
#define DECLARE_FUNCPTR(FUNC) \
    using qiree_##FUNC##_t = std::add_pointer_t<decltype(qiree_##FUNC)>;

DECLARE_FUNCPTR(create);
DECLARE_FUNCPTR(load_module_from_memory);
DECLARE_FUNCPTR(load_module_from_file);
DECLARE_FUNCPTR(num_quantum_reg);
DECLARE_FUNCPTR(num_classical_reg);
DECLARE_FUNCPTR(max_result_items);
DECLARE_FUNCPTR(setup_executor);
DECLARE_FUNCPTR(execute);
DECLARE_FUNCPTR(save_result_items);
DECLARE_FUNCPTR(destroy);
#undef DECLARE_FUNCPTR

class CQireeTest : public ::qiree::test::Test
{
  protected:
    void SetUp() override
    {
        lib_handle_ = dlopen(cqiree_library_path, RTLD_LAZY);
        ASSERT_NE(lib_handle_, nullptr)
            << "Failed to load libcqiree: " << dlerror();

#define LOAD_FUNCPTR(FUNC)                           \
    FUNC##_fn_ = reinterpret_cast<qiree_##FUNC##_t>( \
        dlsym(lib_handle_, "qiree_" #FUNC));         \
    ASSERT_NE(FUNC##_fn_, nullptr)                   \
        << "Failed to load " << #FUNC << ": " << dlerror();
        LOAD_FUNCPTR(create);
        LOAD_FUNCPTR(load_module_from_memory);
        LOAD_FUNCPTR(load_module_from_file);
        LOAD_FUNCPTR(num_quantum_reg);
        LOAD_FUNCPTR(num_classical_reg);
        LOAD_FUNCPTR(max_result_items);
        LOAD_FUNCPTR(setup_executor);
        LOAD_FUNCPTR(execute);
        LOAD_FUNCPTR(save_result_items);
        LOAD_FUNCPTR(destroy);
#undef LOAD_FUNCPTR
    }

    void TearDown() override
    {
        if (lib_handle_)
        {
            dlclose(lib_handle_);
            lib_handle_ = nullptr;
        }
    }

    void* lib_handle_ = nullptr;
    qiree_create_t create_fn_ = nullptr;
    qiree_load_module_from_memory_t load_module_from_memory_fn_ = nullptr;
    qiree_load_module_from_file_t load_module_from_file_fn_ = nullptr;
    qiree_num_quantum_reg_t num_quantum_reg_fn_ = nullptr;
    qiree_num_classical_reg_t num_classical_reg_fn_ = nullptr;
    qiree_max_result_items_t max_result_items_fn_ = nullptr;
    qiree_setup_executor_t setup_executor_fn_ = nullptr;
    qiree_execute_t execute_fn_ = nullptr;
    qiree_save_result_items_t save_result_items_fn_ = nullptr;
    qiree_destroy_t destroy_fn_ = nullptr;
};

TEST_F(CQireeTest, CreateAndDestroy)
{
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);
    destroy_fn_(manager);
}

TEST_F(CQireeTest, LoadModuleFromMemory)
{
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Simple QIR module content
    std::string qir_content = [this] {
        using buf_iter = std::istreambuf_iterator<char>;
        std::ifstream infile{this->test_data_path("minimal.ll")};
        QIREE_ASSERT(infile);
        return std::string{buf_iter{infile}, buf_iter{}};
    }();

    QireeReturnCode result = load_module_from_memory_fn_(
        manager, qir_content.data(), qir_content.size());
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Test invalid inputs
    result = load_module_from_memory_fn_(
        nullptr, qir_content.data(), qir_content.size());
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = load_module_from_memory_fn_(manager, nullptr, 100);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    destroy_fn_(manager);
}

TEST_F(CQireeTest, LoadModuleFromFile)
{
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    auto minimal_filename = this->test_data_path("minimal.ll");

    QireeReturnCode result
        = load_module_from_file_fn_(manager, minimal_filename.c_str());
    EXPECT_EQ(result, QIREE_SUCCESS);

    // Test invalid inputs
    result = load_module_from_file_fn_(nullptr, minimal_filename.c_str());
    EXPECT_EQ(result, QIREE_NOT_READY);

    result = load_module_from_file_fn_(manager, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = load_module_from_file_fn_(manager, "nonexistent_file.ll");
    EXPECT_EQ(result, QIREE_FAIL_LOAD);

    // Clean up
    destroy_fn_(manager);
}

TEST_F(CQireeTest, QueryRegisters)
{
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    QireeReturnCode result = load_module_from_file_fn_(
        manager, this->test_data_path("pyqir_several_gates.ll").c_str());
    ASSERT_EQ(result, QIREE_SUCCESS);

    int num_qubits = 0;
    result = num_quantum_reg_fn_(manager, &num_qubits);
    EXPECT_EQ(result, QIREE_SUCCESS);
    EXPECT_EQ(num_qubits, 4);

    int num_classical = 0;
    result = num_classical_reg_fn_(manager, &num_classical);
    EXPECT_EQ(result, QIREE_SUCCESS);
    EXPECT_EQ(num_classical, 4);

    // Test invalid inputs
    result = num_quantum_reg_fn_(nullptr, &num_qubits);
    EXPECT_EQ(result, QIREE_NOT_READY);

    result = num_quantum_reg_fn_(manager, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = num_classical_reg_fn_(nullptr, &num_classical);
    EXPECT_EQ(result, QIREE_NOT_READY);

    result = num_classical_reg_fn_(manager, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    destroy_fn_(manager);
}

TEST_F(CQireeTest, Run)
{
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    QireeReturnCode result = load_module_from_file_fn_(
        manager, this->test_data_path("teleport.ll").c_str());
    ASSERT_EQ(result, QIREE_SUCCESS);

    size_t max_items = 0;
    result = max_result_items_fn_(manager, 1000, &max_items);
    EXPECT_EQ(result, QIREE_SUCCESS);
    ASSERT_EQ(max_items, 8 + 1);  // = 2^3 (num results) + 1 (size)

    // Test invalid inputs
    result = max_result_items_fn_(nullptr, 1000, &max_items);
    EXPECT_EQ(result, QIREE_NOT_READY);

    result = max_result_items_fn_(manager, -1, &max_items);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = max_result_items_fn_(manager, 1000, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    // Setup executor and run
    result = setup_executor_fn_(manager, "qsim", nullptr);
    ASSERT_EQ(result, QIREE_SUCCESS);

    result = execute_fn_(manager, 100);
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Allocate memory for results
    std::vector<CQireeResultRecord> results(100);

    // Save results
    result = save_result_items_fn_(manager, results.data(), results.size());
    EXPECT_EQ(result, QIREE_SUCCESS);

    // First entry contains the number of results
    EXPECT_EQ(results[0].key, 0);
    EXPECT_EQ(results[0].count, 4);

    // For ease of processing, reinterpret the results as C++ data structures
    using ArrayUint2 = std::array<std::uint64_t, 2>;
    std::vector<ArrayUint2> arr_results(
        reinterpret_cast<ArrayUint2*>(results.data() + 1),
        reinterpret_cast<ArrayUint2*>(results.data() + 1 + results[0].count));
    // For reproducibility, since the results are not in any specific order,
    // sort by key
    std::sort(arr_results.begin() + 1, arr_results.end());
    EXPECT_EQ((std::vector<ArrayUint2>{{0, 21}, {1, 32}, {2, 25}, {3, 22}}),
              arr_results);

    // Test invalid inputs
    result = save_result_items_fn_(nullptr, results.data(), max_items);
    EXPECT_EQ(result, QIREE_NOT_READY);

    result = save_result_items_fn_(manager, nullptr, max_items);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = save_result_items_fn_(manager, results.data(), 1);
    EXPECT_EQ(result, QIREE_FAIL_LOAD);

    destroy_fn_(manager);
}
