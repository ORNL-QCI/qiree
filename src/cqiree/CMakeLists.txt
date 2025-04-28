#----------------------------------*-CMake-*----------------------------------#
# Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#-----------------------------------------------------------------------------#

qiree_add_library(cqiree MODULE
  CQiree.cc
  QireeManager.cc
)
target_link_libraries(cqiree
  PRIVATE
    qiree
)

#----------------------------------------------------------------------------#
# HEADERS
#----------------------------------------------------------------------------#

# C/C++ source headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cqiree"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  COMPONENT development
  FILES_MATCHING REGEX ".*\\.hh$"
)

#-----------------------------------------------------------------------------#
