#---------------------------------*-CMake-*----------------------------------#
# Copyright 2023 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

qiree_configure_file("qiree_version.h.in" "qiree_version.h" @ONLY)
qiree_configure_file("qiree_config.h.in" "qiree_config.h" @ONLY)

#----------------------------------------------------------------------------#
# LIBRARIES
#----------------------------------------------------------------------------#

add_subdirectory(qiree)
add_subdirectory(cqiree)

if(QIREE_USE_XACC)
  add_subdirectory(qirxacc)
endif()

if(QIREE_USE_QSIM)
  add_subdirectory(qirqsim)
endif()

if(QIREE_USE_LIGHTNING)
  add_subdirectory(qirlightning)
endif()

#---------------------------------------------------------------------------##
