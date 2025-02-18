//----------------------------------*-C++-*----------------------------------//
// Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/RecordedResult.hh
//---------------------------------------------------------------------------//
#pragma once

#include <string>
#include <vector>

#include "Types.hh"

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Accumulate sequential measurement outcomes from a single "shot".
 *
 * Each call to result_record_output appends a measurement bit (derived from
 * the provided Result) and its associated label. The storage is preallocated
 * via array_record_output.
 */
//---------------------------------------------------------------------------//

class RecordedResult
{
  public:
    //!@{
    //! \name Type aliases
    using VecBits = std::vector<bool>;
    using VecString = std::vector<std::string>;
    //!@}

  public:
    RecordedResult() = default;

    // Construct with size and optional label
    RecordedResult(std::size_t count, OptionalCString label);

    explicit RecordedResult(std::size_t count) : RecordedResult{count, nullptr}
    {
    }

    inline void push_back(QState result, OptionalCString label = nullptr);

    //! Accessors
    std::string const& container_label() const { return container_label_; }
    VecBits const& bits() const { return bits_; }
    VecString const& entry_labels() const { return entry_labels_; }

  private:
    std::string container_label_;
    VecBits bits_;
    VecString entry_labels_;
};

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
/*!
 * Preallocate storage for measurement results.
 *
 * \param count  Number of measurement bits expected.
 * \param label  Label for the container
 */
RecordedResult::RecordedResult(std::size_t count, OptionalCString label)
{
    if (label)
    {
        container_label_ = label;
    }
    bits_.reserve(count);
    entry_labels_.reserve(count);
}

//---------------------------------------------------------------------------//
/*!
 * Record a measurement result and its optional label.
 */
void RecordedResult::push_back(QState state, OptionalCString label)
{
    bits_.push_back(static_cast<bool>(state));
    entry_labels_.push_back(label ? label : std::string{});
}

//---------------------------------------------------------------------------//
}  // namespace qiree
