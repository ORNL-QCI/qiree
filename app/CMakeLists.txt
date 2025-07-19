#----------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and otrher QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#-----------------------------------------------------------------------------#

FetchContent_Declare(
  # Command Line Parser for C++ programs
  cli11_proj
  QUIET
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG 6c7b07a878ad834957b98d0f9ce1dbe0cb204fc9 # v2.4.2
)

FetchContent_MakeAvailable(cli11_proj)

#-----------------------------------------------------------------------------#
# QSIM FRONT END
#-----------------------------------------------------------------------------#

if(QIREE_USE_QSIM)
  qiree_add_executable(qir-qsim
    qir-qsim.cc
  )
  target_link_libraries(qir-qsim
    PUBLIC QIREE::qiree QIREE::qirqsim
    PRIVATE CLI11::CLI11
  )
endif()

#-----------------------------------------------------------------------------#
# LIGHTNING FRONT END
#-----------------------------------------------------------------------------#

if(QIREE_USE_LIGHTNING)
  qiree_add_executable(qir-lightning
    qir-lightning.cc
  )
  target_link_libraries(qir-lightning
    PUBLIC QIREE::qiree QIREE::qirlightning
    PRIVATE CLI11::CLI11
  )
endif()

#-----------------------------------------------------------------------------#
# XACC FRONT END
#-----------------------------------------------------------------------------#

if(QIREE_USE_XACC)
  qiree_add_executable(qir-xacc
    qir-xacc.cc
  )
  target_link_libraries(qir-xacc
    PUBLIC QIREE::qiree QIREE::qirxacc
    PRIVATE CLI11::CLI11
  )
endif()

#-----------------------------------------------------------------------------#
