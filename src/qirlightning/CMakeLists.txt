#---------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

# Fetch Catalyst runtime include files
include(FetchContent)

include("${CMAKE_CURRENT_SOURCE_DIR}/support_catalyst.cmake")
FindCatalyst(qirlightning)

# Set the path to the lightning simulator shared library
if(DEFINED ENV{LIGHTNING_SIM_PATH})
  set(RTDLIB_PATH "$ENV{LIGHTNING_SIM_PATH}")
  message(STATUS "RTDLIB_PATH set from environment variable LIGHTNING_SIM_PATH: ${RTDLIB_PATH}")
else()
  # Update hard coded path is not found in environment
  set(RTDLIB_PATH "/home/joseph/work/qiree/pennylane-lightning/build_lightning_kokkos/liblightning_kokkos_catalyst.so")
  message(STATUS "RTDLIB_PATH set to default value: ${RTDLIB_PATH}")
endif()

# Set the device name for the lightning simulator
execute_process(
    COMMAND nm -DC "${RTDLIB_PATH}" | grep " Factory"
    OUTPUT_VARIABLE GREP_OUTPUT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

if(GREP_OUTPUT)
  string(REGEX MATCH "T (.*)Factory" SYMBOL_MATCH "${GREP_OUTPUT}")
  if(SYMBOL_MATCH)
    string(REGEX REPLACE "T (.*)Factory" "\\1" RTDDEVICE_NAME "${SYMBOL_MATCH}")
    message(STATUS "Found Lightning Simulator. Extracted RTDDEVICE_NAME: ${RTDDEVICE_NAME}")
  else()
    message(WARNING "Symbol 'Factory' found, but regex failed to extract.")
  endif()
else()
  message(WARNING "Symbol 'Factory' not found in ${RTDLIB_PATH}")
endif()


# Adding lightning as a library to qiree
qiree_add_library(qirlightning
LightningQuantum.cc
LightningDefaultRuntime.cc
)

target_compile_definitions(qirlightning PRIVATE
    RTDLIB="${RTDLIB_PATH}"
    RTDDEVICE="${RTDDEVICE_NAME}"
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
