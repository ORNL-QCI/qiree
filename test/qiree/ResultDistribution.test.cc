//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/ResultDistribution.test.cc
//---------------------------------------------------------------------------//
#include "qiree/ResultDistribution.hh"

#include "qiree/Assert.hh"
#include "qiree/RecordedResult.hh"
#include "qiree_test.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//

// Test normal accumulation and count.
TEST(ResultDistributionTest, AccumulateAndCount)
{
    ResultDistribution dist;
    RecordedResult r1({true, false, true});  // "101"
    RecordedResult r2({true, false, true});  // "101"
    RecordedResult r3({false, true, true});  // "011"

    dist.accumulate(r1);
    dist.accumulate(r2);
    dist.accumulate(r3);

    EXPECT_EQ(dist.count("101"), 2u);
    EXPECT_EQ(dist.count("011"), 1u);
}

// Test that accumulating a RecordedResult with different bit-length throws.
TEST(ResultDistributionTest, AccumulateDifferentLengthThrows)
{
    ResultDistribution dist;
    RecordedResult r1({true, false, true});  // size 3
    RecordedResult r_invalid({false, true, true, false});  // size 4

    dist.accumulate(r1);
    EXPECT_THROW(dist.accumulate(r_invalid), RuntimeError);
}

// Test that count() throws for an invalid key length.
TEST(ResultDistributionTest, CountInvalidKeyLengthThrows)
{
    ResultDistribution dist;
    RecordedResult r1({true, false, true});  // size 3
    dist.accumulate(r1);

    // Key of length 2 instead of 3.
    EXPECT_THROW(dist.count("10"), RuntimeError);
}

// Test that count() throws for a key with invalid characters.
TEST(ResultDistributionTest, CountInvalidKeyCharacterThrows)
{
    ResultDistribution dist;
    RecordedResult r1({true, false, true});  // size 3
    dist.accumulate(r1);

    // Key contains an invalid character 'a'
    EXPECT_THROW(dist.count("10a"), RuntimeError);
}

// Test that count() returns zero for a key that has not been accumulated.
TEST(ResultDistributionTest, CountNotFoundReturnsZero)
{
    ResultDistribution dist;
    RecordedResult r1({true, false, true});  // "101"
    dist.accumulate(r1);

    // Valid key length ("000") but not present in the distribution.
    EXPECT_EQ(dist.count("000"), 0u);
}

// Test to_json produces a JSON string containing the expected key/value pairs.
TEST(ResultDistributionTest, ToJsonProducesValidJson)
{
    ResultDistribution dist;
    RecordedResult r1({true, false, true});  // "101"
    RecordedResult r2({true, false, true});  // "101"
    RecordedResult r3({false, true, true});  // "011"

    dist.accumulate(r1);
    dist.accumulate(r2);
    dist.accumulate(r3);

    std::string json = dist.to_json();
    // Check that both keys and counts appear in the JSON output.
    EXPECT_NE(json.find("\"101\":2"), std::string::npos);
    EXPECT_NE(json.find("\"011\":1"), std::string::npos);
}

// Test behavior on an empty distribution (no accumulation).
TEST(ResultDistributionTest, CountOnEmptyDistribution)
{
    ResultDistribution dist;
    // Without any accumulation, key_length_ is 0.
    // The only valid key is the empty string.
    EXPECT_EQ(dist.count(""), 0u);
    EXPECT_THROW(dist.count("0"), RuntimeError);
}

// Test encode_bit_string for basic functionality
TEST(EncodeBitString, encoding)
{
    auto encode = [](std::string bits) -> std::uint64_t {
        std::uint64_t result = 0;
        encode_bit_string(bits, result);
        return result;
    };
    
    
    // Test single bits
    EXPECT_EQ(encode("0"), 0u);
    EXPECT_EQ(encode("1"), 1u);
    
    // Test multi-bit (little-endian) patterns
    EXPECT_EQ(encode("10"), 1u);
    EXPECT_EQ(encode("01"), 2u);
    EXPECT_EQ(encode("1010"), 5u);
    EXPECT_EQ(encode("11010"), 11u);
    
    // Test maximum length (64 bits)
    std::string max_bits(64, '1');
    EXPECT_EQ(encode(max_bits), UINT64_MAX);
}

// Test encode_bit_string with invalid inputs
TEST(ResultDistributionTest, EncodeBitStringInvalidInputs)
{
    std::uint64_t result = 0;

    // Test empty string (should be 0)
    EXPECT_THROW(encode_bit_string("", result), RuntimeError);
    
    // Test with invalid characters
    EXPECT_THROW(encode_bit_string("102", result), RuntimeError);
    EXPECT_THROW(encode_bit_string("1a1", result), RuntimeError);
    EXPECT_THROW(encode_bit_string("1 1", result), RuntimeError);
    
    // Test with too many bits
    std::string too_many_bits(65, '1');
    EXPECT_THROW(encode_bit_string(too_many_bits, result), RuntimeError);
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
