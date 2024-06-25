//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Types.hh
//---------------------------------------------------------------------------//
#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

namespace qiree
{
//---------------------------------------------------------------------------//
//! Unsigned integer used for QIR
using size_type = std::uint64_t;

//! Integer used for Pauli type
using pauli_type = std::int_least8_t;

//---------------------------------------------------------------------------//
/*!
 * A type-safe wrapper class for managing "pointers".
 *
 * The opaque pointers in QIR are *indices* (or other opaque values) rather
 * than *addresses*:
 *  > Qubits are represented as pointers to an opaque LLVM structure type,
 *  > `%Qubit`. This is done so that qubit values may be distinguished from
 *  > other value types. It is not expected that qubit values actually be valid
 *  > memory addresses, and neither user code nor runtime code should ever
 *  > attempt to dereference a qubit value.
 */
template<class T>
struct OpaqueId
{
    size_type value{0};
};

//---------------------------------------------------------------------------//
/*!
 * Entry point attributes.
 *
 * See
 * https://github.com/qir-alliance/qir-spec/blob/main/specification/under_development/profiles/Base_Profile.md#attributes
 */
struct EntryPointAttrs
{
    size_type required_num_qubits{};
    size_type required_num_results{};
    std::string output_labeling_schema;
    std::string qir_profiles;
};

//---------------------------------------------------------------------------//
/*!
 * Flags describing the QIR version of a loaded module.
 */
struct ModuleFlags
{
    std::int32_t qir_major_version{};
    std::int32_t qir_minor_version{};
    bool dynamic_qubit_management{};
    bool dynamic_result_management{};
};

//---------------------------------------------------------------------------//
// ENUMERATIONS
//---------------------------------------------------------------------------//
/*!
 * Measured quantum state.
 */
enum class QState : bool
{
    zero,  //!< State is |0>
    one  //!< State is |1>
};

//! Pauli basis
enum class Pauli : pauli_type
{
    i = 0,
    x = 1,
    z = 2,
    y = 3,
};

//---------------------------------------------------------------------------//
// TYPE ALIASES
//---------------------------------------------------------------------------//

//! Opaque QIR array identifier
using Array = OpaqueId<struct Array_>;

//! Opaque QIR result identifier
using Result = OpaqueId<struct Result_>;

//! Opaque QIR string identifier
using String = OpaqueId<struct String_>;

//! Opaque QIR tuple identifier
using Tuple = OpaqueId<struct Tuple_>;

//! Opaque QIR qubit identifier
using Qubit = OpaqueId<struct Qubit_>;

//! Pointer to a C string that may be null
using OptionalCString = char const*;

//---------------------------------------------------------------------------//
}  // namespace qiree
