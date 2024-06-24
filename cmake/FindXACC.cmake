#----------------------------------*-CMake-*----------------------------------#
# Copyright 2023 UT-Battelle, LLC
# See the top-level COPYRIGHT file for details.
#[=======================================================================[.rst:

FindXACC
--------

Find the XACC quantum complier interface.


IMPORTED targets
^^^^^^^^^^^^^^^^

``XACC::xacc``
  Main xacc library

Result variables
^^^^^^^^^^^^^^^^

This module will set the following variables if found:

``XACC_FOUND``
  TRUE if found


#]=======================================================================]

include(FindPackageHandleStandardArgs)

if(XACC_DIR AND NOT XACC_ROOT)
  # Backward compatibility: look for root file
  set(XACC_ROOT "${XACC_DIR}")
endif()

find_package(XACC QUIET CONFIG)
find_package_handle_standard_args(XACC CONFIG_MODE)

if(NOT TARGET XACC::xacc)
  # Alias ``xacc::xacc`` to the correctly cased namespace with include
  # proparties
  add_library(XACC::xacc INTERFACE IMPORTED)
  target_link_libraries(XACC::xacc INTERFACE xacc::xacc)
  target_include_directories(XACC::xacc INTERFACE "${XACC_INCLUDE_ROOT}")
endif()


#-----------------------------------------------------------------------------#
