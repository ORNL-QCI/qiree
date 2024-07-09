#---------------------------------*-CMake-*----------------------------------#
# Copyright 2023 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}" QIREE_SOURCE_DIR)
configure_file(qiree_test_config.h.in qiree_test_config.h @ONLY)

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
  target_link_libraries(${_target}
    QIREE::${ns}
    qiree_test_lib
    GTest::gtest_main
  )
  add_test(NAME test/${ns}/${name} COMMAND "$<TARGET_FILE:${_target}>")
endfunction()

#---------------------------------------------------------------------------##
# QIREE TESTS
#---------------------------------------------------------------------------##

qiree_add_test(qiree Executor)
qiree_add_test(qiree Module)

#---------------------------------------------------------------------------##
# QIRXACC TESTS
#---------------------------------------------------------------------------##

if(QIREE_USE_XACC AND QIREE_BUILD_XACC_TESTS)
  qiree_add_test(qirxacc XaccQuantum)
endif()

#---------------------------------------------------------------------------##
