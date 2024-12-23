#---------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

# Adding qsim as a library to qiree
qiree_add_library(qirqsim
  QsimQuantum.cc
  QsimDefaultRuntime.cc
)

#Link the qsim library to qiree and any other relevant libraries
target_link_libraries(qirqsim
  PUBLIC QIREE::qiree  # Link to qiree
  PRIVATE QIREE::qsim
)

#----------------------------------------------------------------------------#
# HEADERS
#----------------------------------------------------------------------------#

# Install headers, matching the relevant .hh files for qsim integration
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/qirqsim"
  COMPONENT development
  FILES_MATCHING REGEX ".*\\.hh?$"
)
