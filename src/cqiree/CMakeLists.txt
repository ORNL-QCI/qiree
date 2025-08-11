#----------------------------------*-CMake-*----------------------------------#
# Copyright 2025 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#-----------------------------------------------------------------------------#

set(_CQIREE_LIBS)
if(QIREE_USE_XACC)
  list(APPEND _CQIREE_LIBS QIREE::qirxacc)
endif()
if(QIREE_USE_QSIM)
  list(APPEND _CQIREE_LIBS QIREE::qirqsim)
endif()

qiree_add_library(cqiree MODULE
  CQiree.cc
  QireeManager.cc
)
target_link_libraries(cqiree
  PRIVATE
    qiree
  PUBLIC
    ${_CQIREE_LIBS}
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
