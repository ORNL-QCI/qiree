//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/BufferManager.hh
//---------------------------------------------------------------------------//

#include "BufferManager.hh"

#include <optional>
#include <string>
#include <unordered_map>

void BufferManager::updateBuffer(std::string const& qubit,
                                 std::string const& state,
                                 int const& value)
{
    // Insert or update the key-value pair in the buffer
    std::pair<std::string, std::string> searchKey = {qubit, state};
    int current_frequency = 0;
    auto it = buffer.find(searchKey);
    if (it != buffer.end())
    {
        current_frequency = it->second;
    }
    // Accumulate counts with every shot
    buffer[{qubit, state}] = value + current_frequency;
}

void BufferManager::updateBuffer(std::string const& key, int const& value)
{
    // Insert or update the key-value pair in the buffer
    simple_buffer[key] = value;
}

std::optional<int> BufferManager::getBufferValue(std::string const& qubit,
                                                 std::string const& state) const
{
    std::pair<std::string, std::string> searchKey = {qubit, state};
    auto it = buffer.find(searchKey);
    if (it != buffer.end())
    {
        return it->second;  // Key found
    }
    return std::nullopt;  // Key not found
}

std::optional<int> BufferManager::getBufferValue(std::string const& key) const
{
    auto it = simple_buffer.find(key);
    if (it != simple_buffer.end())
    {
        return it->second;  // Key found
    }
    return std::nullopt;  // Key not found
}
