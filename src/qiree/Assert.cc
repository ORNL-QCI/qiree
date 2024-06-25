//----------------------------------*-C++-*----------------------------------//
// Copyright 2020-2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file qiree/Assert.cc
//---------------------------------------------------------------------------//
#include "Assert.hh"

#include <cstdlib>
#include <sstream>
#include <utility>

#include "Macros.hh"

#if 0
#    include "detail/ColorUtils.hh"
#endif

namespace qiree
{
namespace
{
//---------------------------------------------------------------------------//
/*!
 * Return whether to give an extra verbose message.
 */
bool determine_verbose_message()
{
#if QIREE_DEBUG
    // Always verbose if debug flags are enabled
    return true;
#else
    // Verbose if the QIREE_LOG environment variable is defined
    return !std::getenv("QIREE_LOG").empty();
#endif
}

char const* color_code(char c)
{
    // TODO: add color codes
    QIREE_DISCARD(c);
    return "";
}

//---------------------------------------------------------------------------//
/*!
 * Construct a debug assertion message for printing.
 */
std::string build_debug_error_msg(DebugErrorDetails const& d)
{
    std::ostringstream msg;
    // clang-format off
    msg << color_code('W') << d.file << ':' << d.line << ':'
        << color_code(' ') << "\nqiree: "
        << color_code('R') << to_cstring(d.which);
    // clang-format on
    if (d.which != DebugErrorType::unreachable)
    {
        msg << ": " << color_code('x') << d.condition;
    }
    msg << color_code(' ');
    return msg.str();
}

//---------------------------------------------------------------------------//
/*!
 * Construct a runtime assertion message for printing.
 */
std::string build_runtime_error_msg(RuntimeErrorDetails const& d)
{
    static bool const verbose_message = determine_verbose_message();

    std::ostringstream msg;

    if (d.which != RuntimeErrorType::validate || verbose_message)
    {
        msg << color_code('W') << d.file;
        if (d.line)
        {
            msg << ':' << d.line;
        }
        msg << ':' << color_code(' ') << '\n';
    }

    msg << "qir-ee: " << color_code('R') << to_cstring(d.which) << " error: ";
    if (verbose_message || d.what.empty())
    {
        msg << color_code('x') << d.condition << color_code(' ') << " failed";
        if (!d.what.empty())
            msg << ":\n    ";
    }
    else
    {
        msg << color_code(' ');
    }
    msg << d.what;

    return msg.str();
}
//---------------------------------------------------------------------------//
}  // namespace

//---------------------------------------------------------------------------//
/*!
 * Get a human-readable string describing a debug error.
 */
char const* to_cstring(DebugErrorType which)
{
    switch (which)
    {
        case DebugErrorType::precondition:
            return "precondition failed";
        case DebugErrorType::internal:
            return "internal assertion failed";
        case DebugErrorType::unreachable:
            return "unreachable code point";
        case DebugErrorType::unconfigured:
            return "required dependency is disabled in this build";
        case DebugErrorType::unimplemented:
            return "feature is not yet implemented";
        case DebugErrorType::postcondition:
            return "postcondition failed";
        case DebugErrorType::assumption:
            return "assumption failed";
    }
    return "";
}

//---------------------------------------------------------------------------//
/*!
 * Get a human-readable string describing a runtime error.
 */
char const* to_cstring(RuntimeErrorType which)
{
    switch (which)
    {
        case RuntimeErrorType::validate:
            return "runtime";
    }
    return "";
}

//---------------------------------------------------------------------------//
/*!
 * Construct a debug exception from detailed attributes.
 */
DebugError::DebugError(DebugErrorDetails d)
    : std::logic_error(build_debug_error_msg(d)), details_(std::move(d))
{
}

//---------------------------------------------------------------------------//
/*!
 * Construct a runtime exception from a validation failure.
 */
RuntimeError RuntimeError::from_validate(std::string what,
                                         char const* code,
                                         char const* file,
                                         int line)
{
    return RuntimeError{{RuntimeErrorType::validate, what, code, file, line}};
}

//---------------------------------------------------------------------------//
/*!
 * Construct a runtime error from detailed descriptions.
 */
RuntimeError::RuntimeError(RuntimeErrorDetails d)
    : std::runtime_error(build_runtime_error_msg(d)), details_(std::move(d))
{
}

//---------------------------------------------------------------------------//
}  // namespace qiree
