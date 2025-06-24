//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/ResultDistribution.hh
//---------------------------------------------------------------------------//
#pragma once

#include <string>
#include <unordered_map>

namespace qiree
{
class RecordedResult;
//---------------------------------------------------------------------------//
/*!
 * Distribution of recorded results from shots.
 * The distribution is formed by accumulating multiple \c RecordedResult
 * instances into a sparse map of {bit string -> count}. The length of the bit
 * string is the same for all keys. We provide accessors by little-endian
 * string key such as \c "01001" which corresponds to a bit string \code
 * {false, true, false, false, true}
 * \endcode
 * Little-endian is an "array-like" representation (the initial
 * character corresponds to the zeroth qubit) rather than a "number-like"
 * representation (i.e., a base-2 encoding of the qubits). It is serializable
 * to JSON, creating an object that has bit strings as keys and counts as
 * values.
 */
class ResultDistribution
{
  public:
    // Accumulate the results from a RecordedResult instance.
    // Throws if the bit-length of the new result
    // differs from previously accumulated ones.
    void accumulate(RecordedResult const& result);

    // Access the count for a given bit string key (e.g. "01001").
    // Returns 0 if the key is not present.
    std::size_t count(std::string const& key) const;

    // Serialize the distribution to a JSON string.
    // The JSON object has bit string keys and count values.
    std::string to_json() const;

    //!@{
    //! Iterate over the nonzero keys
    auto begin() const { return distribution_.begin(); }
    auto end() const { return distribution_.end(); }
    //!@}

    //! Get the number of nonzero entries
    auto size() const throw() { return distribution_.size(); }

  private:
    // Sparse map of {bit string -> count}
    std::unordered_map<std::string, std::size_t> distribution_;

    // key_length_ is 0 until the first RecordedResult is accumulated.
    // All subsequent RecordedResult instances must have the same bit length.
    std::size_t key_length_ = 0;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
