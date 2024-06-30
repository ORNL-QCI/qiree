#---------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

qiree_add_library(qirxacc
  XaccQuantum.cc
  XaccDefaultRuntime.cc
  XaccTupleRuntime.cc
)
target_link_libraries(qirxacc
  PUBLIC QIREE::qiree
  PRIVATE XACC::xacc
)

#----------------------------------------------------------------------------#
# HEADERS
#----------------------------------------------------------------------------#

# C++ source headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/qirxacc"
  COMPONENT development
  FILES_MATCHING REGEX ".*\\.hh?$"
)

#---------------------------------------------------------------------------##
