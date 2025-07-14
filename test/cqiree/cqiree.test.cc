//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/cqiree.test.cc
//---------------------------------------------------------------------------//
#pragma once

#include <gtest/gtest.h>
#include <dlfcn.h>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Define the function signatures matching the CQiree.h interface
typedef struct CQiree_s CQiree;

typedef enum
{
    QIREE_SUCCESS = 0,
    QIREE_NOT_READY,
    QIREE_INVALID_INPUT,
    QIREE_FAIL_LOAD,
    QIREE_FAIL_EXECUTE
} QireeReturnCode;

typedef struct
{
    uint64_t key;
    uint64_t count;
} CQireeResultRecord;

typedef CQiree* (*qiree_create_t)();
typedef QireeReturnCode (*qiree_load_module_from_memory_t)(CQiree*, const char*, size_t);
typedef QireeReturnCode (*qiree_load_module_from_file_t)(CQiree*, const char*);
typedef QireeReturnCode (*qiree_num_quantum_reg_t)(CQiree*, int*);
typedef QireeReturnCode (*qiree_num_classical_reg_t)(CQiree*, int*);
typedef QireeReturnCode (*qiree_max_result_items_t)(CQiree*, int, size_t*);
typedef QireeReturnCode (*qiree_setup_executor_t)(CQiree*, const char*, const char*);
typedef QireeReturnCode (*qiree_execute_t)(CQiree*, int);
typedef QireeReturnCode (*qiree_save_result_items_t)(CQiree*, size_t, CQireeResultRecord*);
typedef void (*qiree_destroy_t)(CQiree*);

class CQireeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load the shared library
        #ifdef __APPLE__
        lib_handle_ = dlopen("libcqiree.dylib", RTLD_LAZY);
        #else
        lib_handle_ = dlopen("libcqiree.so", RTLD_LAZY);
        #endif

        ASSERT_NE(lib_handle_, nullptr) << "Failed to load libcqiree: " << dlerror();

        // Get function pointers
        create_fn_ = reinterpret_cast<qiree_create_t>(dlsym(lib_handle_, "qiree_create"));
        ASSERT_NE(create_fn_, nullptr) << "Failed to load qiree_create: " << dlerror();

        load_from_memory_fn_ = reinterpret_cast<qiree_load_module_from_memory_t>(
            dlsym(lib_handle_, "qiree_load_module_from_memory"));
        ASSERT_NE(load_from_memory_fn_, nullptr) << "Failed to load qiree_load_module_from_memory: " << dlerror();

        load_from_file_fn_ = reinterpret_cast<qiree_load_module_from_file_t>(
            dlsym(lib_handle_, "qiree_load_module_from_file"));
        ASSERT_NE(load_from_file_fn_, nullptr) << "Failed to load qiree_load_module_from_file: " << dlerror();

        num_quantum_reg_fn_ = reinterpret_cast<qiree_num_quantum_reg_t>(
            dlsym(lib_handle_, "qiree_num_quantum_reg"));
        ASSERT_NE(num_quantum_reg_fn_, nullptr) << "Failed to load qiree_num_quantum_reg: " << dlerror();

        num_classical_reg_fn_ = reinterpret_cast<qiree_num_classical_reg_t>(
            dlsym(lib_handle_, "qiree_num_classical_reg"));
        ASSERT_NE(num_classical_reg_fn_, nullptr) << "Failed to load qiree_num_classical_reg: " << dlerror();

        max_result_items_fn_ = reinterpret_cast<qiree_max_result_items_t>(
            dlsym(lib_handle_, "qiree_max_result_items"));
        ASSERT_NE(max_result_items_fn_, nullptr) << "Failed to load qiree_max_result_items: " << dlerror();

        setup_executor_fn_ = reinterpret_cast<qiree_setup_executor_t>(
            dlsym(lib_handle_, "qiree_setup_executor"));
        ASSERT_NE(setup_executor_fn_, nullptr) << "Failed to load qiree_setup_executor: " << dlerror();

        execute_fn_ = reinterpret_cast<qiree_execute_t>(
            dlsym(lib_handle_, "qiree_execute"));
        ASSERT_NE(execute_fn_, nullptr) << "Failed to load qiree_execute: " << dlerror();

        save_result_items_fn_ = reinterpret_cast<qiree_save_result_items_t>(
            dlsym(lib_handle_, "qiree_save_result_items"));
        ASSERT_NE(save_result_items_fn_, nullptr) << "Failed to load qiree_save_result_items: " << dlerror();

        destroy_fn_ = reinterpret_cast<qiree_destroy_t>(
            dlsym(lib_handle_, "qiree_destroy"));
        ASSERT_NE(destroy_fn_, nullptr) << "Failed to load qiree_destroy: " << dlerror();
    }

    void TearDown() override {
        if (lib_handle_) {
            dlclose(lib_handle_);
            lib_handle_ = nullptr;
        }
    }

    // Helper to create a temporary file with content
    std::string createTempFile(const std::string& content) {
        std::string filename = "temp_qir_module_" +
            std::to_string(reinterpret_cast<uintptr_t>(this)) + ".ll";

        std::ofstream file(filename);
        file << content;
        file.close();

        return filename;
    }

    void* lib_handle_ = nullptr;
    qiree_create_t create_fn_ = nullptr;
    qiree_load_module_from_memory_t load_from_memory_fn_ = nullptr;
    qiree_load_module_from_file_t load_from_file_fn_ = nullptr;
    qiree_num_quantum_reg_t num_quantum_reg_fn_ = nullptr;
    qiree_num_classical_reg_t num_classical_reg_fn_ = nullptr;
    qiree_max_result_items_t max_result_items_fn_ = nullptr;
    qiree_setup_executor_t setup_executor_fn_ = nullptr;
    qiree_execute_t execute_fn_ = nullptr;
    qiree_save_result_items_t save_result_items_fn_ = nullptr;
    qiree_destroy_t destroy_fn_ = nullptr;
};

TEST_F(CQireeTest, CreateAndDestroy) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);
    destroy_fn_(manager);
}

TEST_F(CQireeTest, LoadModuleFromMemory) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Simple QIR module content
    const char* qir_content = R"(
        ; ModuleID = 'simple_bell'
        define void @main() {
          ret void
        }
    )";

    QireeReturnCode result = load_from_memory_fn_(manager, qir_content, strlen(qir_content));
    EXPECT_EQ(result, QIREE_SUCCESS);

    // Test invalid inputs
    result = load_from_memory_fn_(nullptr, qir_content, strlen(qir_content));
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = load_from_memory_fn_(manager, nullptr, 100);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    destroy_fn_(manager);
}

TEST_F(CQireeTest, LoadModuleFromFile) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Create a temporary file with QIR content
    std::string qir_content = R"(
        ; ModuleID = 'simple_bell'
        define void @main() {
          ret void
        }
    )";

    std::string filename = createTempFile(qir_content);

    QireeReturnCode result = load_from_file_fn_(manager, filename.c_str());
    EXPECT_EQ(result, QIREE_SUCCESS);

    // Test invalid inputs
    result = load_from_file_fn_(nullptr, filename.c_str());
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = load_from_file_fn_(manager, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = load_from_file_fn_(manager, "nonexistent_file.ll");
    EXPECT_EQ(result, QIREE_FAIL_LOAD);

    // Clean up
    std::remove(filename.c_str());
    destroy_fn_(manager);
}

TEST_F(CQireeTest, QueryRegisters) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Load a simple module first
    const char* qir_content = R"(
        ; ModuleID = 'bell_pair'
        define void @main() {
          ; Contains 2 qubits and 1 classical register
          ret void
        }
    )";

    QireeReturnCode result = load_from_memory_fn_(manager, qir_content, strlen(qir_content));
    ASSERT_EQ(result, QIREE_SUCCESS);

    int num_qubits = 0;
    result = num_quantum_reg_fn_(manager, &num_qubits);
    EXPECT_EQ(result, QIREE_SUCCESS);
    // The exact number depends on the implementation, but should be non-negative
    EXPECT_GE(num_qubits, 0);

    int num_classical = 0;
    result = num_classical_reg_fn_(manager, &num_classical);
    EXPECT_EQ(result, QIREE_SUCCESS);
    EXPECT_GE(num_classical, 0);

    // Test invalid inputs
    result = num_quantum_reg_fn_(nullptr, &num_qubits);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = num_quantum_reg_fn_(manager, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = num_classical_reg_fn_(nullptr, &num_classical);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = num_classical_reg_fn_(manager, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    destroy_fn_(manager);
}

TEST_F(CQireeTest, MaxResultItems) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Load a simple module first
    const char* qir_content = R"(
        ; ModuleID = 'bell_pair'
        define void @main() {
          ret void
        }
    )";

    QireeReturnCode result = load_from_memory_fn_(manager, qir_content, strlen(qir_content));
    ASSERT_EQ(result, QIREE_SUCCESS);

    size_t max_items = 0;
    result = max_result_items_fn_(manager, 1000, &max_items);
    EXPECT_EQ(result, QIREE_SUCCESS);
    EXPECT_GT(max_items, 0);

    // Test invalid inputs
    result = max_result_items_fn_(nullptr, 1000, &max_items);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = max_result_items_fn_(manager, -1, &max_items);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = max_result_items_fn_(manager, 1000, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    destroy_fn_(manager);
}

TEST_F(CQireeTest, ExecutionWorkflow) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Load a simple module first
    const char* qir_content = R"(
        ; ModuleID = 'bell_pair'
        define void @main() {
          ret void
        }
    )";

    QireeReturnCode result = load_from_memory_fn_(manager, qir_content, strlen(qir_content));
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Setup executor
    result = setup_executor_fn_(manager, "default", nullptr);
    EXPECT_EQ(result, QIREE_SUCCESS);

    // With config
    result = setup_executor_fn_(manager, "default", "{\"shots\": 1000}");
    EXPECT_EQ(result, QIREE_SUCCESS);

    // Execute with shots
    result = execute_fn_(manager, 100);
    EXPECT_EQ(result, QIREE_SUCCESS);

    // Test invalid inputs
    result = setup_executor_fn_(nullptr, "default", nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = setup_executor_fn_(manager, nullptr, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = execute_fn_(nullptr, 100);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = execute_fn_(manager, -1);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    destroy_fn_(manager);
}

TEST_F(CQireeTest, SaveResultItems) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Load a simple module first
    const char* qir_content = R"(
        ; ModuleID = 'bell_pair'
        define void @main() {
          ret void
        }
    )";

    QireeReturnCode result = load_from_memory_fn_(manager, qir_content, strlen(qir_content));
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Setup executor and run
    result = setup_executor_fn_(manager, "default", nullptr);
    ASSERT_EQ(result, QIREE_SUCCESS);

    result = execute_fn_(manager, 100);
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Get max items needed
    size_t max_items = 0;
    result = max_result_items_fn_(manager, 100, &max_items);
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Allocate memory for results
    std::vector<CQireeResultRecord> results(max_items);

    // Save results
    result = save_result_items_fn_(manager, max_items, results.data());
    EXPECT_EQ(result, QIREE_SUCCESS);

    // Check that the results are valid
    // First entry contains the count of results
    EXPECT_GT(results[0].count, 0);

    // Test invalid inputs
    result = save_result_items_fn_(nullptr, max_items, results.data());
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = save_result_items_fn_(manager, 0, results.data());
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    result = save_result_items_fn_(manager, max_items, nullptr);
    EXPECT_EQ(result, QIREE_INVALID_INPUT);

    destroy_fn_(manager);
}

TEST_F(CQireeTest, EndToEndWorkflow) {
    CQiree* manager = create_fn_();
    ASSERT_NE(manager, nullptr);

    // Load a simple module
    const char* qir_content = R"(
        ; ModuleID = 'bell_pair'
        define void @main() {
          ; Simple Bell pair circuit
          ret void
        }
    )";

    QireeReturnCode result = load_from_memory_fn_(manager, qir_content, strlen(qir_content));
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Query registers
    int num_qubits = 0;
    result = num_quantum_reg_fn_(manager, &num_qubits);
    ASSERT_EQ(result, QIREE_SUCCESS);

    int num_classical = 0;
    result = num_classical_reg_fn_(manager, &num_classical);
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Setup and run
    result = setup_executor_fn_(manager, "default", "{\"optimize\": true}");
    ASSERT_EQ(result, QIREE_SUCCESS);

    const int num_shots = 1000;
    result = execute_fn_(manager, num_shots);
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Get results
    size_t max_items = 0;
    result = max_result_items_fn_(manager, num_shots, &max_items);
    ASSERT_EQ(result, QIREE_SUCCESS);

    std::vector<CQireeResultRecord> results(max_items);
    result = save_result_items_fn_(manager, max_items, results.data());
    ASSERT_EQ(result, QIREE_SUCCESS);

    // Check results
    size_t total_shots = 0;
    for (size_t i = 1; i <= results[0].count; ++i) {
        total_shots += results[i].count;
    }
    EXPECT_EQ(total_shots, num_shots);

    destroy_fn_(manager);
}
