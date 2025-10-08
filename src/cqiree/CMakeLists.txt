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

if(BUILD_SHARED_LIBS)
  # When building with shared libs, this should be a shared library that can
  # be dlopen()'d.
  set(_LIB_TYPE MODULE)
else()
  set(_LIB_TYPE)
endif()

qiree_add_library(cqiree ${_LIB_TYPE}
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
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cqiree"
  COMPONENT development
  FILES_MATCHING REGEX ".*\\.h$"
)

#-----------------------------------------------------------------------------#
