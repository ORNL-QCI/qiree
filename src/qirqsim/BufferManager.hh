//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirqsim/BufferManager.hh
//---------------------------------------------------------------------------//

#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <unordered_map>
#include <string>
#include <optional>
#include <functional>
#include <utility>

// Define a hash function for std::pair

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        // Combine the two hash values
        return hash1 ^ (hash2 << 1);  // Shift and XOR
    }
};

class BufferManager {
public:
    
    // Method to update the buffer with a key-value pair
    void updateBuffer(const std::string& qubit, const std::string& state, const int& value);
    
    // Retrieve buffer value for storage or evaluation
    std::optional<int> getBufferValue(const std::string& qubit, const std::string& state) const;
    
private:
    
    // Dictionary to store key-value pairs
    std::unordered_map<std::pair<std::string, std::string>, int, pair_hash> buffer;
};

#endif // BUFFER_MANAGER_H
