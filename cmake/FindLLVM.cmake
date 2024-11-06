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

# Check if the system is macOS
if(APPLE)
  # Set LLVM_DIR to the Homebrew location if using macOS
  set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm" CACHE PATH "Path to LLVM on macOS")
endif()

find_package(LLVM QUIET CONFIG)
find_package_handle_standard_args(LLVM CONFIG_MODE)

if(LLVM_FOUND)
  add_library(LLVM::headers INTERFACE IMPORTED)
  target_include_directories(LLVM::headers SYSTEM INTERFACE
    "${LLVM_INCLUDE_DIRS}"
  )
else()
  message(WARNING "Could not find LLVM. Make sure LLVM is installed and LLVM_DIR is set.")
endif()

#-----------------------------------------------------------------------------#

