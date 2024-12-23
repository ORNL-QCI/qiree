//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Buffer.hh
//---------------------------------------------------------------------------//

#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

namespace qiree
{

// Define a hash function for std::pair

struct pair_hash
{
    template<class T1, class T2>
    std::size_t operator()(std::pair<T1, T2> const& pair) const
    {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        // Combine the two hash values
        return hash1 ^ (hash2 << 1);  // Shift and XOR
    }
};

class Buffer
{
  public:
    // Method to update the buffer with a key-value pair
    // TODO: Don't use strings here
    void updateBuffer(std::string const& qubit,
                      std::string const& state,
                      int const& value);
    void updateBuffer(std::string const& key, int const& value);

    // Retrieve buffer value for storage or evaluation
    std::optional<int>
    getBufferValue(std::string const& qubit, std::string const& state) const;
    std::optional<int> getBufferValue(std::string const& key) const;

  private:
    // Dictionary to store key-value pairs
    std::unordered_map<std::pair<std::string, std::string>, int, pair_hash> buffer;
    std::unordered_map<std::string, int> simple_buffer;
};

// BUFFER_H

} // namespace qiree








