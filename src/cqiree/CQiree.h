//----------------------------------*-C-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/CQiree.h
//---------------------------------------------------------------------------//
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/* Opaque pointer to QireeManager */
typedef struct CQiree_s CQiree;

/* Return codes */
enum QireeReturnCode
{
    QIREE_SUCCESS = 0,
    QIREE_FAIL_LOAD = 1,
    /* Add other return codes as needed */
};

/* Create and destroy QireeManager instance */
CQiree* qiree_create();
void qiree_destroy(CQiree* manager);

/* Module loading functions */
int qiree_load_module_from_memory(CQiree* manager,
                                  char const* data_contents,
                                  size_t length);
int qiree_load_module_from_file(CQiree* manager, char const* filename);

/* Register query functions */
int qiree_num_quantum_reg(CQiree* manager, int* result);
int qiree_num_classical_reg(CQiree* manager, int* result);

/* Amount of memory needed to store result (bytes) */
int qiree_sizeof_result(CQiree* manager, int num_shots, int* result);

/* Executor setup and execution */
int qiree_setup_executor(CQiree* manager,
                         char const* backend,
                         char const* config_json);

int qiree_execute(CQiree* manager, int num_shots);

/* Encoded result: first entry is # pairs, every following pair is (bitstring,
 * count) [123, 0, 3, 15, 10, ...] 123: number of following entries 0:
 * 000000000 bitstring 3: number of samples of "0000000" 15: 00001111 bitstring
 * 10: number of samples
 * ....{ 123, (), ()... }
 */
int qiree_save_result(CQiree* manager, size_t max, uint64_t* encoded);

#ifdef __cplusplus
}
#endif