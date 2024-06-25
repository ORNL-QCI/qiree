#---------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

llvm_map_components_to_libnames(_llvm_libs
  Core
  irreader # loading QIR
  MCJIT native # execution engine (JIT compilation)
)

#----------------------------------------------------------------------------#
# LIBRARIES
#----------------------------------------------------------------------------#

qiree_add_library(qiree
  Assert.cc
  Module.cc
  Executor.cc
  QuantumNotImpl.cc
)
target_compile_features(qiree PUBLIC cxx_std_17)
target_link_libraries(qiree
  PRIVATE
    ${_llvm_libs} LLVM::headers
)
target_include_directories(qiree
  PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${QIREE_HEADER_CONFIG_DIRECTORY}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

#----------------------------------------------------------------------------#
# HEADERS
#----------------------------------------------------------------------------#

# C++ source headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/qiree"
  COMPONENT development
  FILES_MATCHING REGEX ".*\\.hh?$"
)

#---------------------------------------------------------------------------##
