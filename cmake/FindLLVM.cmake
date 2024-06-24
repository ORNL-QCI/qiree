#----------------------------------*-CMake-*----------------------------------#
# Copyright 2023 UT-Battelle, LLC
# See the top-level COPYRIGHT file for details.
#[=======================================================================[.rst:

FindLLVM
--------

Find the LLVM complier interface.


IMPORTED targets
^^^^^^^^^^^^^^^^

``LLVM::``
  Main xacc library

Result variables
^^^^^^^^^^^^^^^^

This module will set the following variables if found:

``LLVM_FOUND``
  TRUE if found


#]=======================================================================]

include(FindPackageHandleStandardArgs)

find_package(LLVM QUIET CONFIG)
find_package_handle_standard_args(LLVM CONFIG_MODE)

#-----------------------------------------------------------------------------#

