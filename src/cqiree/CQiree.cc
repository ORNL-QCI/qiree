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

extern "C" {

CQiree* qiree_create()
{
    return reinterpret_cast<CQiree*>(new qiree::QireeManager());
}

void qiree_destroy(CQiree* manager)
{
    delete reinterpret_cast<qiree::QireeManager*>(manager);
}

int qiree_load_module_from_memory(CQiree* manager,
                                  char const* data_contents,
                                  size_t length)
{
    if (!manager || !data_contents)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);
    return static_cast<int>(
        cpp_manager->load_module(std::string_view(data_contents, length)));
}

int qiree_load_module_from_file(CQiree* manager, char const* filename)
{
    if (!manager || !filename)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);
    return static_cast<int>(cpp_manager->load_module(std::string(filename)));
}

int qiree_num_quantum_reg(CQiree* manager, int* result)
{
    if (!manager || !result)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);
    return static_cast<int>(cpp_manager->num_quantum_reg(*result));
}

int qiree_num_classical_reg(CQiree* manager, int* result)
{
    if (!manager || !result)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);
    return static_cast<int>(cpp_manager->num_classical_reg(*result));
}

int qiree_setup_executor(CQiree* manager,
                         char const* backend,
                         char const* config_json)
{
    if (!manager || !backend)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);

    std::string_view backend_sv(backend);
    std::string_view config_sv = config_json ? std::string_view(config_json)
                                             : std::string_view();

    return static_cast<int>(cpp_manager->setup_executor(backend_sv, config_sv));
}

int qiree_execute(CQiree* manager, int num_shots)
{
    if (!manager)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);
    return static_cast<int>(cpp_manager->execute(num_shots));
}

int qiree_num_results(CQiree* manager, int* count)
{
    if (!manager || !count)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);
    return static_cast<int>(cpp_manager->num_results(*count));
}

int qiree_get_result(CQiree* manager, int index, char const* key, int* count)
{
    if (!manager || !key || !count)
        return static_cast<int>(qiree::QireeManager::ReturnCode::fail_load);

    auto* cpp_manager = reinterpret_cast<qiree::QireeManager*>(manager);
    return static_cast<int>(
        cpp_manager->get_result(index, std::string_view(key), count));
}

}  // extern "C"