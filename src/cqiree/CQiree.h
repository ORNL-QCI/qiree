//----------------------------------*-C-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/CQiree.h
//---------------------------------------------------------------------------//
#pragma once

#include <cstdint>
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/* Opaque pointer to QireeManager */
typedef struct CQiree_s CQiree;

/* Return codes */
typedef enum
{
    QIREE_SUCCESS = 0,
    QIREE_NOT_READY,
    QIREE_INVALID_INPUT,
    QIREE_FAIL_LOAD,
    QIREE_FAIL_EXECUTE
} QireeReturnCode;

/* Return items */
typedef struct
{
    uint64_t key;
    uint64_t count;
} CQireeResultRecord;

/* Create a QireeManager instance */
CQiree* qiree_create();

/* Module loading functions */
QireeReturnCode qiree_load_module_from_memory(CQiree* manager,
                                              char const* data_contents,
                                              size_t length);
QireeReturnCode
qiree_load_module_from_file(CQiree* manager, char const* filename);

/* Register query functions */
QireeReturnCode qiree_num_quantum_reg(CQiree* manager, int* result);
QireeReturnCode qiree_num_classical_reg(CQiree* manager, int* result);

/* Number of records needed to store result, including capacity */
QireeReturnCode
qiree_max_result_items(CQiree* manager, int num_shots, size_t* result);

/* Executor setup and execution: config_json may be null */
QireeReturnCode qiree_setup_executor(CQiree* manager,
                                     char const* backend,
                                     char const* config_json);

QireeReturnCode qiree_execute(CQiree* manager, int num_shots);

/*
 * Encoded result (N + 1 capacity for N items):
 * - first pair is the number of following pairs
 * - every following integer is a (bitstring, count) pair
 * - the bitstring is "little endian" with shifting: qubit N is
 *  `(value >> N) & 1` for N in [0, 64)
 *
 * Example: [xxx, 123, 0, 3, 15, 10, ...]
 * - xxx: ignored
 * - 123: number of following entries
 * - 0: 000000000 bitstring
 * - 3: number of samples of "0000000"
 * - 15: 00001111 bitstring
 * - 10: number of samples
 */
QireeReturnCode qiree_save_result_items(CQiree* manager,
                                        CQireeResultRecord* encoded,
                                        size_t encoded_size);

/* Cleanly destroy a QireeManager instance */
void qiree_destroy(CQiree* manager);

#ifdef __cplusplus
}
#endif
