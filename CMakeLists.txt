#---------------------------------*-CMake-*----------------------------------#
# Copyright 2023 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.12)

# Set QIREE_VERSION using git tags using the following format
set(CGV_TAG_REGEX "v([0-9.]+)(-dev|-rc.[0-9]+)?")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/CgvFindVersion.cmake")
cgv_find_version(QIREE)

project(QIREE VERSION "${QIREE_VERSION}" LANGUAGES CXX)
cmake_policy(VERSION 3.12...3.22)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/QIREEUtils.cmake")

macro(qiree_set_default name value)
  if(NOT DEFINED ${name})
    message(VERBOSE "QIR-EE set default ${name}=${value}")
    set(${name} "${value}")
  endif()
  list(APPEND QIREE_DEFAULT_VARIABLES ${name})
endmacro()

#----------------------------------------------------------------------------#
# MAIN OPTIONS
#----------------------------------------------------------------------------#

# Components
option(QIREE_BUILD_DOCS "Build QIR-EE documentation" OFF)
option(QIREE_BUILD_TESTS "Build QIR-EE unit tests" ON)
option(QIREE_USE_XACC "Build XACC interface" OFF)
qiree_set_default(BUILD_TESTING ${QIREE_BUILD_TESTS})

# Assertion handling
option(QIREE_DEBUG "Enable runtime assertions" ON)

#----------------------------------------------------------------------------#
# CMAKE INTRINSIC OPTIONS
#
# These are generally used to initialize properties on targets, and it's
# possible QIR-EE is being built inside another project. Instead of saving
# these as cache variables (which change the defaults project-wide, including
# changing behavior of other code that was loaded before this project) set them
# as # local variables to be inherited underneath this directory.
#----------------------------------------------------------------------------#

### Configuration ###
qiree_set_default(CMAKE_EXPORT_NO_PACKAGE_REGISTRY ON)
qiree_set_default(CMAKE_FIND_USE_PACKAGE_REGISTRY FALSE)
qiree_set_default(CMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY FALSE)

### Build flags ###
# Default to debug or released based on value of QIREE_DEBUG
if(DEFINED CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE)
  if(QIREE_DEBUG)
    set(_default_build_type "Debug")
  else()
    set(_default_build_type "Release")
  endif()
  set(CMAKE_BUILD_TYPE "${_default_build_type}" CACHE STRING "Build type" FORCE)
  message(STATUS "Set default CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
  unset(_default_build_type)
endif()
# Default to using C++17 everywhere
qiree_set_default(CMAKE_CXX_STANDARD 17)
qiree_set_default(CMAKE_CXX_EXTENSIONS OFF)

### Linking flags ###
# Default to building shared libraries (*not* a cache variable)
qiree_set_default(BUILD_SHARED_LIBS ON)
# Inform installed binaries of external library rpaths
qiree_set_default(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
if(BUILD_SHARED_LIBS)
  # Inform installed binaries of internal library rpaths
  qiree_set_default(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
  # Do not relink libs/binaries when dependent shared libs change
  qiree_set_default(CMAKE_LINK_DEPENDS_NO_SHARED ON)
endif()

### Installation flags ###
# When developing add checking for proper usage of `install(`
if(QIREE_DEBUG)
  qiree_set_default(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION ON)
endif()
# Avoid printing details about already installed files
qiree_set_default(CMAKE_INSTALL_MESSAGE LAZY)

#----------------------------------------------------------------------------#
# UNUSED: Output locations for QIR-EE products
set(QIREE_CMAKE_CONFIG_DIRECTORY
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake")
set(QIREE_HEADER_CONFIG_DIRECTORY
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
set(QIREE_LIBRARY_OUTPUT_DIRECTORY
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(QIREE_RUNTIME_OUTPUT_DIRECTORY
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")

#----------------------------------------------------------------------------#
# DEPENDENCIES
#----------------------------------------------------------------------------#

enable_language(C) # Needed for LLVM
find_package(LLVM REQUIRED)
if((LLVM_VERSION VERSION_LESS 14)
  OR (LLVM_VERSION VERSION_GREATER 17.1))
  message(WARNING "QIR-EE is only tested with LLVM 14-17: found version ${LLVM_VERSION}")
endif()

if(QIREE_USE_XACC)
  find_package(XACC REQUIRED)
endif()

if(QIREE_BUILD_DOCS)
  if(NOT Doxygen_FOUND)
    find_package(Doxygen)
  endif()
  if(NOT Doxygen_FOUND)
    message(SEND_ERROR
      "Doxygen is required for building documentation but was not found"
    )
  endif()

  if(NOT Python_FOUND)
    find_package(Python REQUIRED)
  endif()
  set(QIREE_PYTHONPATH "$ENV{PYTHONPATH}" CACHE STRING
    "Python path used for finding modules and generating documentation"
  )

  qiree_check_python_module(QIREE_USE_Sphinx sphinx)
  set(Sphinx_FOUND ${QIREE_USE_Sphinx})
  if(Sphinx_FOUND)
    qiree_check_python_module(QIREE_USE_Breathe breathe)
    qiree_check_python_module(QIREE_USE_Furo "furo")
    qiree_check_python_module(QIREE_USE_SphinxBibtex "sphinxcontrib.bibtex")
    if(QIREE_USE_Breathe)
      find_program(LATEXMK_EXECUTABLE latexmk)
    endif()
  endif()
  include(ExternalProject)
endif()

if(QIREE_BUILD_TESTS AND NOT GTest_FOUND)
  find_package(GTest)
  if(NOT GTest_FOUND)
    message(SEND_ERROR
      "Googletest (GTest) is required for testing but was not found"
    )
  endif()
endif()

#----------------------------------------------------------------------------#
# LIBRARY
#----------------------------------------------------------------------------#

# Add the main libraries
add_subdirectory(src)

#----------------------------------------------------------------------------#
# UNIT TESTS
#----------------------------------------------------------------------------#

if(QIREE_BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif()

#----------------------------------------------------------------------------#
# APPLICATIONS AND BINARIES
#----------------------------------------------------------------------------#

add_subdirectory(app)

#----------------------------------------------------------------------------#
# DOCUMENTATION
#----------------------------------------------------------------------------#

if(QIREE_BUILD_DOCS)
  add_subdirectory(doc)
endif()

#----------------------------------------------------------------------------#
# CONFIG FILE INSTALLATION
#----------------------------------------------------------------------------#

# Where to install configured cmake files
set(QIREE_INSTALL_CMAKECONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/QIREE")

set(_cmake_files
  "${CMAKE_CURRENT_LIST_DIR}/cmake/FindLLVM.cmake"
  "${CMAKE_CURRENT_LIST_DIR}/cmake/FindXACC.cmake"
)
install(FILES ${_cmake_files}
  DESTINATION "${QIREE_INSTALL_CMAKECONFIGDIR}"
  COMPONENT development
)

# Export all cache variables that start with QIREE_
set(QIREE_EXPORT_VARIABLES)
macro(qiree_export_var varname)
  list(APPEND QIREE_EXPORT_VARIABLES "set(${varname} \"${${varname}}\")")
endmacro()
qiree_export_var(QIREE_VERSION_STRING)
list(APPEND QIREE_EXPORT_VARIABLES "\n# Configuration options")
get_directory_property(_cachevar_keys CACHE_VARIABLES)
foreach(_key IN LISTS _cachevar_keys)
  if(_key MATCHES "^QIREE_")
    qiree_export_var(${_key})
  endif()
endforeach()

# Export defaulted variables
list(APPEND QIREE_EXPORT_VARIABLES "\n# Defaulted variables")
foreach(_key IN LISTS QIREE_DEFAULT_VARIABLES)
  list(APPEND QIREE_EXPORT_VARIABLES "set(QIREE_${_key} \"${${_key}}\")")
endforeach()

# Add hints for direct dependencies and indirect geant dependencies
list(APPEND QIREE_EXPORT_VARIABLES "\n# Hints for upstream dependencies")
foreach(_key LLVM_DIR GTest_DIR XACC_DIR)
  set(_val "${${_key}}")
  if(_val)
    list(APPEND QIREE_EXPORT_VARIABLES
      "if(NOT DEFINED ${_key})"
      "  set(${_key} \"${_val}\")"
      "endif()"
    )
  endif()
endforeach()
list(JOIN QIREE_EXPORT_VARIABLES "\n" QIREE_EXPORT_VARIABLES)

# Generate the file needed by downstream "find_package(QIREE)"
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/QIREEConfig.cmake.in"
  "${QIREE_CMAKE_CONFIG_DIRECTORY}/QIREEConfig.cmake"
  @ONLY
)

# Export version info
# TODO for version 1.0.0: change COMPATIBILITY to MajorVersion
write_basic_package_version_file(
  "${QIREE_CMAKE_CONFIG_DIRECTORY}/QIREEConfigVersion.cmake"
  COMPATIBILITY AnyNewerVersion
)

# Install generated config files
install(DIRECTORY "${QIREE_HEADER_CONFIG_DIRECTORY}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  COMPONENT development
  FILES_MATCHING REGEX ".*\\.hh?$"
)

# Install the config and version files
install(FILES
  "${QIREE_CMAKE_CONFIG_DIRECTORY}/QIREEConfig.cmake"
  "${QIREE_CMAKE_CONFIG_DIRECTORY}/QIREEConfigVersion.cmake"
  DESTINATION ${QIREE_INSTALL_CMAKECONFIGDIR}
  COMPONENT development
)

# Install 'QIREETargets.cmake', included by QIREEConfig.cmake, which
# references the targets we install.
install(EXPORT qiree-targets
  FILE QIREETargets.cmake
  NAMESPACE QIREE::
  DESTINATION "${QIREE_INSTALL_CMAKECONFIGDIR}"
  COMPONENT development
)

# Uncomment when v0.1.0 is tagged
# if(QIREE_VERSION VERSION_EQUAL "0.0.0")
#   install(CODE "
# message(WARNING \"The QIR-EE version was not detected during configuration.
#   (Check the beginning of your initial configure output for more details.)
#   This will result in the installation having incorrect version metadata and
#   will interfere with downstream CMake version requirements and may obscure
#   provenance data in output results.\")
# ")
# endif()

#----------------------------------------------------------------------------#
