//----------------------------------*-C++-*----------------------------------//
// Copyright 2020-2024 UT-Battelle, LLC, and other Celeritas developers.
// See https://github.com/celeritas-project/celeritas/blob/develop/COPYRIGHT
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
/*!
 * \file qiree/Assert.hh
 * \brief Macros, exceptions, and helpers for assertions and error handling.
 *
 * This defines host- and device-compatible assertion macros that are toggled
 * on the \c QIREE_DEBUG configure macro.
 */
//---------------------------------------------------------------------------//
#pragma once

#include <ostream>  // IWYU pragma: export
#include <sstream>  // IWYU pragma: keep
#include <stdexcept>
#include <string>

#include "qiree_config.h"

#include "Macros.hh"

//---------------------------------------------------------------------------//
// MACROS
//---------------------------------------------------------------------------//
/*!
 * \def QIREE_EXPECT
 *
 * Precondition debug assertion macro. We "expect" that the input values
 * or initial state satisfy a precondition, and we throw exception in
 * debug mode if they do not.
 */
/*!
 * \def QIREE_ASSERT
 *
 * Internal debug assertion macro. This replaces standard \c assert usage.
 */
/*!
 * \def QIREE_ENSURE
 *
 * Postcondition debug assertion macro. Use to "ensure" that return values or
 * side effects are as expected when leaving a function.
 */
/*!
 * \def QIREE_ASSUME
 *
 * Always-on compiler assumption. This should be used very rarely: you should
 * make sure the resulting assembly is simplified in optimize mode from using
 * the assumption. For example, sometimes informing the compiler of an
 * assumption can reduce code bloat by skipping standard library exception
 * handling code (e.g. in \c std::visit by assuming \c
 * !var_obj.valueless_by_exception() ).
 */
/*!
 * \def QIREE_VALIDATE
 *
 * Always-on runtime assertion macro. This can check user input and input data
 * consistency, and will raise RuntimeError on failure with a descriptive error
 * message that is streamed as the second argument. This macro cannot be used
 * in \c __device__ -annotated code.
 *
 * The error message should read: \verbatim
   "<PROBLEM> (<WHY IT'S A PROBLEM>) <SUGGESTION>?"
  \endverbatim
 *
 * Examples with correct casing and punctuation:
 * - "failed to open '{filename}' (should contain relaxation data)"
 * - "unexpected end of file '{filename}' (data is inconsistent with
 *   boundaries)"
 * - "MPI was not initialized (needed to construct a communicator). Maybe set
 *   the environment variable QIREE_DISABLE_PARALLEL=1 to disable
 *   externally?"
 * - "invalid min_range={opts.min_range} (must be positive)"
 *
 * This looks in pracice like:
 * \code
   QIREE_VALIDATE(file_stream,
                  << "failed to open '" << filename
                  << "' (should contain relaxation data)");
 * \endcode
 *
 * An always-on debug-type assertion without a detailed message can be
 * constructed by omitting the stream (but leaving the comma):
 * \code
    QIREE_VALIDATE(file_stream,);
 * \endcode
 */
/*!
 * \def QIREE_DEBUG_FAIL
 *
 * Throw a debug assertion regardless of the \c QIREE_DEBUG setting. This
 * is used internally but is also useful for catching subtle programming errors
 * in downstream code.
 */
/*!
 * \def QIREE_ASSERT_UNREACHABLE
 *
 * Throw an assertion if the code point is reached. When debug assertions are
 * turned off, this changes to a compiler hint that improves optimization (and
 * may force the coded to exit uncermoniously if the point is encountered,
 * rather than continuing on with undefined behavior).
 */
/*!
 * \def QIREE_NOT_CONFIGURED
 *
 * Assert if the code point is reached because an optional feature is disabled.
 * This generally should be used for the constructors of dummy class
 * definitions in, e.g., \c Foo.nocuda.cc:
 * \code
    Foo::Foo()
    {
        QIREE_NOT_CONFIGURED("CUDA");
    }
 * \endcode
 */
/*!
 * \def QIREE_NOT_IMPLEMENTED
 *
 * Assert if the code point is reached because a feature has yet to be fully
 * implemented.
 *
 * This placeholder is so that code paths can be "declared but not defined" and
 * implementations safely postponed in a greppable manner. This should \em not
 * be used to define "unused" overrides for virtual classes. A correct use case
 * would be:
 * \code
   if (z > AtomicNumber{26})
   {
       QIREE_NOT_IMPLEMENTED("physics for heavy nuclides");
   }
 * \endcode
 */

//! \cond

#if !defined(__HIP__) && !defined(__CUDA_ARCH__)
// Throw in host code
#    define QIREE_DEBUG_THROW_(MSG, WHICH) \
        throw ::qiree::DebugError(         \
            {::qiree::DebugErrorType::WHICH, MSG, __FILE__, __LINE__})
#elif defined(__CUDA_ARCH__) && !defined(NDEBUG)
// Use the assert macro for CUDA when supported
#    define QIREE_DEBUG_THROW_(MSG, WHICH) \
        assert(false && sizeof(#WHICH ": " MSG))
#else
// Use a special device function to emulate assertion failure if HIP
// (assertion from multiple threads simultaeously can cause unexpected device
// failures on AMD hardware) or if NDEBUG is in use with CUDA
#    define QIREE_DEBUG_THROW_(MSG, WHICH) \
        ::qiree::device_debug_error(       \
            ::qiree::DebugErrorType::WHICH, MSG, __FILE__, __LINE__)
#endif

#define QIREE_DEBUG_ASSERT_(COND, WHICH)      \
    do                                        \
    {                                         \
        if (QIREE_UNLIKELY(!(COND)))          \
        {                                     \
            QIREE_DEBUG_THROW_(#COND, WHICH); \
        }                                     \
    } while (0)
#define QIREE_NDEBUG_ASSUME_(COND)   \
    do                               \
    {                                \
        if (QIREE_UNLIKELY(!(COND))) \
        {                            \
            ::qiree::unreachable();  \
        }                            \
    } while (0)
#define QIREE_NOASSERT_(COND)   \
    do                          \
    {                           \
        if (false && (COND)) {} \
    } while (0)
//! \endcond

#define QIREE_DEBUG_FAIL(MSG, WHICH)    \
    do                                  \
    {                                   \
        QIREE_DEBUG_THROW_(MSG, WHICH); \
        ::qiree::unreachable();         \
    } while (0)

#if QIREE_DEBUG
#    define QIREE_EXPECT(COND) QIREE_DEBUG_ASSERT_(COND, precondition)
#    define QIREE_ASSERT(COND) QIREE_DEBUG_ASSERT_(COND, internal)
#    define QIREE_ENSURE(COND) QIREE_DEBUG_ASSERT_(COND, postcondition)
#    define QIREE_ASSUME(COND) QIREE_DEBUG_ASSERT_(COND, assumption)
#    define QIREE_ASSERT_UNREACHABLE() \
        QIREE_DEBUG_FAIL("unreachable code point encountered", unreachable)
#else
#    define QIREE_EXPECT(COND) QIREE_NOASSERT_(COND)
#    define QIREE_ASSERT(COND) QIREE_NOASSERT_(COND)
#    define QIREE_ENSURE(COND) QIREE_NOASSERT_(COND)
#    define QIREE_ASSUME(COND) QIREE_NDEBUG_ASSUME_(COND)
#    define QIREE_ASSERT_UNREACHABLE() ::qiree::unreachable()
#endif

#if !QIREE_DEVICE_COMPILE || defined(__HIP__)
#    define QIREE_VALIDATE(COND, MSG)                                     \
        do                                                                \
        {                                                                 \
            if (QIREE_UNLIKELY(!(COND)))                                  \
            {                                                             \
                std::ostringstream qiree_runtime_msg_;                    \
                qiree_runtime_msg_ MSG;                                   \
                throw ::qiree::RuntimeError::from_validate(               \
                    qiree_runtime_msg_.str(), #COND, __FILE__, __LINE__); \
            }                                                             \
        } while (0)
#else
#    define QIREE_VALIDATE(COND, MSG)                                        \
        QIREE_DEBUG_FAIL("QIREE_VALIDATE cannot be called from device code", \
                         unreachable);
#endif

#define QIREE_NOT_CONFIGURED(WHAT) QIREE_DEBUG_FAIL(WHAT, unconfigured)
#define QIREE_NOT_IMPLEMENTED(WHAT) QIREE_DEBUG_FAIL(WHAT, unimplemented)

//---------------------------------------------------------------------------//
// ENUMERATIONS
//---------------------------------------------------------------------------//

namespace qiree
{
//---------------------------------------------------------------------------//
// ENUMERATIONS
//---------------------------------------------------------------------------//
enum class DebugErrorType
{
    precondition,  //!< Precondition contract violation
    internal,  //!< Internal assertion check failure
    unreachable,  //!< Internal assertion: unreachable code path
    unconfigured,  //!< Internal assertion: required feature not enabled
    unimplemented,  //!< Internal assertion: not yet implemented
    postcondition,  //!< Postcondition contract violation
    assumption,  //!< "Assume" violation
};

enum class RuntimeErrorType
{
    validate,  //!< QIR-EE runtime error
};

//! Detailed properties of a debug assertion failure
struct DebugErrorDetails
{
    DebugErrorType which;
    char const* condition;
    char const* file;
    int line;
};

//! Detailed properties of a runtime error
struct RuntimeErrorDetails
{
    RuntimeErrorType which{RuntimeErrorType::validate};
    std::string what{};
    std::string condition{};
    std::string file{};
    int line{0};
};

//---------------------------------------------------------------------------//
// FUNCTIONS
//---------------------------------------------------------------------------//

//! Invoke undefined behavior
[[noreturn]] inline void unreachable()
{
    QIREE_UNREACHABLE;
}

//! Get a pretty string version of a debug error
char const* to_cstring(DebugErrorType which);

//! Get a pretty string version of a runtime error
char const* to_cstring(RuntimeErrorType which);

//---------------------------------------------------------------------------//
// TYPES
//---------------------------------------------------------------------------//
/*!
 * Error thrown by QIR-EE assertions.
 */
class DebugError : public std::logic_error
{
  public:
    // Construct from debug attributes
    explicit DebugError(DebugErrorDetails);

    //! Access the debug data
    DebugErrorDetails const& details() const { return details_; }

  private:
    DebugErrorDetails details_;
};

//---------------------------------------------------------------------------//
/*!
 * Error thrown by working code from unexpected runtime conditions.
 */
class RuntimeError : public std::runtime_error
{
  public:
    // Construct from validation failure
    static RuntimeError
    from_validate(std::string msg, char const* code, char const* file, int line);

    // Construct from details
    explicit RuntimeError(RuntimeErrorDetails);

    //! Access detailed information
    RuntimeErrorDetails const& details() const { return details_; }

  private:
    RuntimeErrorDetails details_;
};

//---------------------------------------------------------------------------//
}  // namespace qiree
