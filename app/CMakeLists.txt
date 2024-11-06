#----------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and otrher QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#-----------------------------------------------------------------------------#

include(FetchContent)
FetchContent_Declare(
  cli11_proj
  QUIET
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git # Command Line Parser for C++ programs
  GIT_TAG f4d0731cebb123ff0ace712c099dffbcd2c58e5a # v2.4.1
)

FetchContent_MakeAvailable(cli11_proj)

# Conditionally add XACC-based executable

if(QIREE_USE_XACC)
  qiree_add_executable(qir-xacc
    qir-xacc.cc
  )
  target_link_libraries(qir-xacc
    PUBLIC QIREE::qiree QIREE::qirxacc
    PRIVATE CLI11::CLI11
  )
endif()

# Conditionally download and configure qsim library

if(QIREE_USE_QSIM)
  FetchContent_Declare(
    qsim_lib
    GIT_REPOSITORY https://github.com/quantumlib/qsim.git
    GIT_TAG master # Use a specific commit/tag if needed
  )
  
  FetchContent_GetProperties(qsim_lib)
  
  if(NOT qsim_lib_POPULATED)
    FetchContent_MakeAvailable(qsim_lib)

    # Copy header files to tpls/qsim
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tpls/qsim)
    message(STATUS "Copying qsim headers to ${CMAKE_SOURCE_DIR}/tpls/qsim")
    file(GLOB qsim_headers "${qsim_lib_SOURCE_DIR}/lib/*.h")
    file(COPY ${qsim_headers} DESTINATION ${CMAKE_SOURCE_DIR}/tpls/qsim)
  endif()

  find_package(OpenMP REQUIRED)

  if(OpenMP_CXX_FOUND)
    target_link_libraries(qirqsim PUBLIC OpenMP::OpenMP_CXX)
  endif()
  # Collect source files for the qsim library
  #file(GLOB SRC "${CMAKE_SOURCE_DIR}/src/qirqsim/*.cc")

  # Add qsim library with the correct include directories

  #add_library(qsim SHARED ${SRC})
  #target_include_directories(qsim 
  #  PUBLIC 
  #    ${CMAKE_SOURCE_DIR}/tpls/qsim            # qsim headers
  #    ${CMAKE_SOURCE_DIR}/tpls/qsim/lib        # Additional qsim headers if needed
  #    )
  
  # Add the qir-qsim executable and link it with qsim
  qiree_add_executable(qir-qsim qir-qsim.cc)
  target_link_libraries(qir-qsim 
    PUBLIC QIREE::qiree QIREE::qirqsim 
    PRIVATE CLI11::CLI11 
  )
endif()

#-----------------------------------------------------------------------------#
