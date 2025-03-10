#---------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

# Include directories for catalyst_runtime
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/catalyst_runtime/include")

# Adding lightning as a library to qiree
qiree_add_library(qirlightning
  LightningQuantum.cc
  LightningDefaultRuntime.cc
)

#Link the lightning library to qiree and any other relevant libraries
target_link_libraries(qirlightning
  PUBLIC QIREE::qiree  # Link to qiree
  PRIVATE QIREE::lightning
)

#----------------------------------------------------------------------------#
# HEADERS
#----------------------------------------------------------------------------#

# Install headers, matching the relevant .hh files for qsim integration
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/qirlightning"
  COMPONENT development
  FILES_MATCHING REGEX ".*\\.hh?$"
)
