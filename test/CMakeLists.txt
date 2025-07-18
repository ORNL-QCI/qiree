#---------------------------------*-CMake-*----------------------------------#
# Copyright 2023 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

# Generate a file with configure-time paths
file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}" QIREE_SOURCE_DIR)
configure_file(qiree_test_config.h.in qiree_test_config.h @ONLY)

# Generate a file with generate-time paths
file(
  GENERATE
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/qiree_targets.h"
  CONTENT [[
#ifndef qiree_targets_h
#define qiree_targets_h

static char const cqiree_library_path[] = "$<TARGET_FILE:cqiree>";

#endif /* qiree_targets_h */
]])

#---------------------------------------------------------------------------##
# LIBRARY
#---------------------------------------------------------------------------##
add_library(qiree_test_lib
  qiree/QuantumTestImpl.cc
  Test.cc
)
target_link_libraries(qiree_test_lib
  PUBLIC QIREE::qiree GTest::GTest
)
target_include_directories(qiree_test_lib
  PUBLIC
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

#---------------------------------------------------------------------------##
# MACROS
#---------------------------------------------------------------------------##
function(qiree_add_test ns name)
  set(_target ${ns}_${name}Test)
  add_executable(${_target}
    ${ns}/${name}.test.cc
  )
  set(_libs
    qiree_test_lib
    GTest::gtest_main
  )
  if(NOT ns STREQUAL "cqiree")
    list(APPEND _libs QIREE::${ns})
  endif()
  target_link_libraries(${_target} ${_libs})
  add_test(NAME test/${ns}/${name} COMMAND "$<TARGET_FILE:${_target}>")
endfunction()

#---------------------------------------------------------------------------##
# QIREE TESTS
#---------------------------------------------------------------------------##

qiree_add_test(qiree Executor)
qiree_add_test(qiree Module)
qiree_add_test(qiree ResultDistribution)

#---------------------------------------------------------------------------##
# CQIREE TESTS
#---------------------------------------------------------------------------##

qiree_add_test(CQiree cqiree)
add_dependencies(cqiree_CQireeTest CQiree)

#---------------------------------------------------------------------------##
# QIRXACC TESTS
#---------------------------------------------------------------------------##

if(QIREE_USE_XACC)
  qiree_add_test(qirxacc XaccQuantum)
endif()

#---------------------------------------------------------------------------##
# QIRQSIM TESTS
#---------------------------------------------------------------------------##

if(QIREE_USE_QSIM)
  qiree_add_test(qirqsim QsimQuantum)
endif()

#---------------------------------------------------------------------------##
