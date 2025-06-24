//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file cqiree/CQiree.cc
//---------------------------------------------------------------------------//

#include "CQiree.h"

#include <string>
#include <string_view>

#include "QireeManager.hh"

using QM = qiree::QireeManager;

extern "C" {

CQiree* qiree_create()
{
    return reinterpret_cast<CQiree*>(new QM());
}

void qiree_destroy(CQiree* manager)
{
    delete reinterpret_cast<QM*>(manager);
}

QireeReturnCode qiree_load_module_from_memory(CQiree* manager,
                                              char const* data_contents,
                                              size_t length)
{
    if (!manager)
        return QIREE_NOT_READY;
    if (!data_contents)
        return QIREE_INVALID_INPUT;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);
    return static_cast<QireeReturnCode>(
        cpp_manager->load_module(std::string_view(data_contents, length)));
}

QireeReturnCode
qiree_load_module_from_file(CQiree* manager, char const* filename)
{
    if (!manager)
        return QIREE_NOT_READY;
    if (!filename)
        return QIREE_INVALID_INPUT;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);
    return static_cast<QireeReturnCode>(
        cpp_manager->load_module(std::string(filename)));
}

QireeReturnCode qiree_num_quantum_reg(CQiree* manager, int* result)
{
    if (!manager)
        return QIREE_NOT_READY;
    if (!result)
        return QIREE_INVALID_INPUT;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);
    return static_cast<QireeReturnCode>(cpp_manager->num_quantum_reg(*result));
}

QireeReturnCode qiree_num_classical_reg(CQiree* manager, int* result)
{
    if (!manager)
        return QIREE_NOT_READY;
    if (!result)
        return QIREE_INVALID_INPUT;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);
    return static_cast<QireeReturnCode>(
        cpp_manager->num_classical_reg(*result));
}

QireeReturnCode qiree_setup_executor(CQiree* manager,
                                     char const* backend,
                                     char const* config_json)
{
    if (!manager)
        return QIREE_NOT_READY;
    if (!backend)
        return QIREE_INVALID_INPUT;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);

    std::string_view backend_sv(backend);
    std::string_view config_sv = config_json ? std::string_view(config_json)
                                             : std::string_view();

    return static_cast<QireeReturnCode>(
        cpp_manager->setup_executor(backend_sv, config_sv));
}

QireeReturnCode qiree_execute(CQiree* manager, int num_shots)
{
    if (!manager)
        return QIREE_NOT_READY;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);
    return static_cast<QireeReturnCode>(cpp_manager->execute(num_shots));
}

QireeReturnCode qiree_num_results(CQiree* manager, int* count)
{
    if (!manager)
        return QIREE_NOT_READY;
    if (!count)
        return QIREE_INVALID_INPUT;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);
    return static_cast<QireeReturnCode>(cpp_manager->num_results(*count));
}

QireeReturnCode
qiree_get_result(CQiree* manager, int index, char const* key, int* count)
{
    if (!manager)
        return QIREE_NOT_READY;
    if (!key || !count)
        return QIREE_NOT_READY;

    auto* cpp_manager = reinterpret_cast<QM*>(manager);
    return static_cast<QireeReturnCode>(
        cpp_manager->get_result(index, std::string_view(key), count));
}

}  // extern "C"