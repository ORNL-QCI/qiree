#----------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and otrher QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#-----------------------------------------------------------------------------#

include(FetchContent)
FetchContent_Declare(
  cli11_proj
  QUIET
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG f4d0731cebb123ff0ace712c099dffbcd2c58e5a # v2.4.1
)

FetchContent_MakeAvailable(cli11_proj)

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
