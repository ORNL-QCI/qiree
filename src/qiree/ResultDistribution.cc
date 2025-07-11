//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/ResultDistribution.cc
//---------------------------------------------------------------------------//
#include "ResultDistribution.hh"

#include <algorithm>

#include "RecordedResult.hh"
#include "qiree/Assert.hh"

namespace qiree
{
namespace
{
//---------------------------------------------------------------------------//
/*!
 * Convert bits to a bit string key ("0" for false, "1" for true).
 */
std::string to_key(std::vector<bool> const& bits)
{
    std::string key;
    key.reserve(bits.size());
    for (bool bit : bits)
    {
        key.push_back(bit ? '1' : '0');
    }
    return key;
}

}  // namespace

//---------------------------------------------------------------------------//
/*!
 * Accumulate a single shot.
 */
void ResultDistribution::accumulate(RecordedResult const& result)
{
    auto const& bits = result.bits();

    if (QIREE_UNLIKELY(key_length_ == 0))
    {
        // This is the first result: store the key length
        key_length_ = bits.size();
    }
    else
    {
        QIREE_VALIDATE(bits.size() == key_length_,
                       << "RecordedResult bit length " << bits.size()
                       << " does not match distribution key length "
                       << key_length_);
    }

    ++distribution_[to_key(bits)];
}

//---------------------------------------------------------------------------//
/*!
 * Access the number of shots that resulted in this bit string.
 */
std::size_t ResultDistribution::count(std::string const& key) const
{
    QIREE_VALIDATE(key.length() == key_length_,
                   << "invalid key length: expected " << key_length_
                   << " but got " << key.length());
    QIREE_VALIDATE(std::all_of(key.begin(),
                               key.end(),
                               [](char c) { return c == '0' || c == '1'; }),
                   << "invalid key: expected only '0' or '1'");

    auto it = distribution_.find(key);
    return (it != distribution_.end()) ? it->second : 0;
}

//---------------------------------------------------------------------------//
/*!
 * Build a JSON string of the distribution.
 */
std::string ResultDistribution::to_json() const
{
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (auto const& kv : distribution_)
    {
        if (!first)
            oss << ",";
        first = false;
        oss << "\"" << kv.first << "\":" << kv.second;
    }
    oss << "}";
    return oss.str();
}

//---------------------------------------------------------------------------//
/*!
 * Encode a bitstring as "little endian" with shifting:
 * register N is  `(value >> N) & 1` for N in [0, 64).
 */
void encode_bit_string(std::string const& key, std::uint64_t& result)
{
    QIREE_VALIDATE(!key.empty() && key.size() <= 64,
                   << "invalid key size: must be between 1 and 64 bits: got "
                   << key.size() << " bits");

    result = 0;

    // Process from least significant bit to most significant
    for (size_t i = 0; i < key.size(); ++i)
    {
        // Validate the character
        QIREE_VALIDATE(key[i] == '0' || key[i] == '1',
                       << "Invalid bit character '" << key[i]
                       << "' at position " << i);

        // Set the bit if it's '1'
        if (key[i] == '1')
        {
            result |= (1ULL << i);
        }
    }
}

//---------------------------------------------------------------------------//
}  // namespace qiree
