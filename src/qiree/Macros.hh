//----------------------------------*-C++-*----------------------------------//
// Copyright 2020-2024 UT-Battelle, LLC, and other Celeritas developers.
// See https://github.com/celeritas-project/celeritas/blob/develop/COPYRIGHT
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
/*!
 * \file qiree/Macros.hh
 * \brief Language and compiler abstraction macro definitions.
 *
 * The Macros file defines cross-platform (CUDA, C++, HIP) macros that
 * expand to attributes depending on the compiler and build configuration.
 */
//---------------------------------------------------------------------------//
#pragma once

#include "qiree_config.h"

//---------------------------------------------------------------------------//
// MACROS
//---------------------------------------------------------------------------//
/*!
 * \def QIREE_FORCEINLINE
 *
 * Force a function to be inline. Compiler optimizers usually can
 * tell what needs optimizing, but this function can provide speedups (and
 * smaller sampling profiles) when inlining optimizations are not enabled. It
 * should be used sparingly.
 */
#define QIREE_FORCEINLINE_FUNCTION QIREE_FUNCTION QIREE_FORCEINLINE

#if defined(__NVCC__)
#    define QIREE_FORCEINLINE __forceinline__
#elif defined(_MSC_VER)
#    define QIREE_FORCEINLINE inline __forceinline
#elif defined(__clang__) || defined(__GNUC__) || defined(__HIP__) \
    || defined(__INTEL_COMPILER)
#    define QIREE_FORCEINLINE inline __attribute__((always_inline))
#else
#    define QIREE_FORCEINLINE inline
#endif

/*!
 * \def QIREE_UNLIKELY(condition)
 *
 * Mark the result of this condition to be "unlikely".
 *
 * This asks the compiler to move the section of code to a "cold" part of the
 * instructions, improving instruction locality. It should be used primarily
 * for error checking conditions.
 */
#if defined(__clang__) || defined(__GNUC__)
// GCC and Clang support the same builtin
#    define QIREE_UNLIKELY(COND) __builtin_expect(!!(COND), 0)
#else
// No other compilers seem to have a similar builtin
#    define QIREE_UNLIKELY(COND) (COND)
#endif

/*!
 * \def QIREE_UNREACHABLE
 *
 * Mark a point in code as being impossible to reach in normal execution.
 *
 * See https://clang.llvm.org/docs/LanguageExtensions.html#builtin-unreachable
 * or https://msdn.microsoft.com/en-us/library/1b3fsfxw.aspx
 * or
 * https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#__builtin_unreachable
 *
 * (The "unreachable" and "assume" compiler optimizations for CUDA are only
 * available in API version 11.3 or higher, which is encoded as
 * \code major*1000 + minor*10 \endcode).
 *
 * \note This macro should not generally be used; instead, the macro \c
 * QIREE_ASSERT_UNREACHABLE() defined in base/Assert.hh should be used instead
 * (to provide a more detailed error message in case the point *is* reached).
 */
#if (!defined(__CUDA_ARCH__) && (defined(__clang__) || defined(__GNUC__))) \
    || defined(__NVCOMPILER)                                               \
    || (defined(__CUDA_ARCH__) && CUDART_VERSION >= 11030)                 \
    || defined(__HIP_DEVICE_COMPILE__)
#    define QIREE_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#    define QIREE_UNREACHABLE __assume(false)
#else
#    define QIREE_UNREACHABLE
#endif

/*!
 * \def QIREE_TRY_HANDLE
 *
 * "Try" to execute the statement, and "handle" *all* thrown errors by calling
 * the given function-like error handler with a \c std::exception_ptr object.
 *
 * \note A file that uses this macro must include the \c \<exception\> header
 * (but since the \c HANDLE_EXCEPTION needs to take an exception pointer, it's
 * got to be included anyway).
 */
#define QIREE_TRY_HANDLE(STATEMENT, HANDLE_EXCEPTION)   \
    do                                                  \
    {                                                   \
        try                                             \
        {                                               \
            STATEMENT;                                  \
        }                                               \
        catch (...)                                     \
        {                                               \
            HANDLE_EXCEPTION(std::current_exception()); \
        }                                               \
    } while (0)

/*!
 * \def QIREE_TRY_HANDLE_CONTEXT
 *
 * Try the given statement, and if it fails, chain it into the given exception.
 *
 * The given \c CONTEXT_EXCEPTION must be an expression that yields an rvalue
 * to a \c std::exception subclass that isn't \c final . The resulting chained
 * exception will be passed into \c HANDLE_EXCEPTION for processing.
 */
#define QIREE_TRY_HANDLE_CONTEXT(                          \
    STATEMENT, HANDLE_EXCEPTION, CONTEXT_EXCEPTION)        \
    QIREE_TRY_HANDLE(                                      \
        do {                                               \
            try                                            \
            {                                              \
                STATEMENT;                                 \
            }                                              \
            catch (...)                                    \
            {                                              \
                std::throw_with_nested(CONTEXT_EXCEPTION); \
            }                                              \
        } while (0),                                       \
        HANDLE_EXCEPTION)

/*!
 * \def QIREE_DEFAULT_COPY_MOVE
 *
 * Explicitly declare defaulted copy and move constructors and assignment
 * operators. Use this if the destructor is declared explicitly.
 */
#define QIREE_DEFAULT_COPY_MOVE(CLS)      \
    CLS(CLS const&) = default;            \
    CLS& operator=(CLS const&) = default; \
    CLS(CLS&&) = default;                 \
    CLS& operator=(CLS&&) = default

/*!
 * \def QIREE_DELETE_COPY_MOVE
 *
 * Explicitly declare *deleted* copy and move constructors and assignment
 * operators. Use this for scoped RAII classes.
 */
#define QIREE_DELETE_COPY_MOVE(CLS)      \
    CLS(CLS const&) = delete;            \
    CLS& operator=(CLS const&) = delete; \
    CLS(CLS&&) = delete;                 \
    CLS& operator=(CLS&&) = delete

/*!
 * \def QIREE_DISCARD
 *
 * The argument is an unevaluated operand which will generate no code but force
 * the expression to be used. This is used in place of the \code
 * [[maybe_unused]] \endcode attribute, which actually generates warnings in
 * older versions of GCC.
 */
#define QIREE_DISCARD(CODE) static_cast<void>(sizeof(CODE));

//---------------------------------------------------------------------------//
