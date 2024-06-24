cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(test)

# Minimum CMake version required

cmake_minimum_required(VERSION 3.22)

# Allow the user to specify the target source file

set(TARGET_SOURCE "llvm_link_test.cpp" CACHE STRING "Target source file")

# Allow the user to specify the LLVM version

set(LLVM_VERSION "16" CACHE STRING "LLVM version to use")

# Project Name

project(test)

# Find LLVM and the components we require.

find_package(LLVM ${LLVM_VERSION} REQUIRED)

llvm_map_components_to_libnames(

  LLVM_LIBRARIES

  Core

  ExecutionEngine

  Interpreter

  MC

  MCJIT

  Support

  nativecodegen

)

# Locate XACC or set its directory

if (NOT XACC_DIR)
    set(XACC_DIR "$ENV{HOME}/.xacc")
endif()

message("XACC_DIR: ${XACC_DIR}")

find_package(XACC REQUIRED)

# Create a wrapper for the LLVM and XACC components we need

add_library(test::LLVM INTERFACE IMPORTED)

target_include_directories(test::LLVM INTERFACE ${LLVM_INCLUDE_DIRS} ${XACC_DIR}/include/xacc)

target_compile_definitions(test::LLVM INTERFACE ${LLVM_DEFINITIONS})

target_link_libraries(test::LLVM INTERFACE ${LLVM_LIBRARIES} xacc)

# Source files

set(PROJECT_SOURCES ../src/GlobalMappingAssociator.cpp ../src/GlobalMappingAssociator.hpp)

# Create executable

add_executable(qir_execute ${TARGET_SOURCE} ${PROJECT_SOURCES})

target_link_directories(qir_execute PRIVATE ${XACC_DIR}/lib)

target_link_libraries(qir_execute PRIVATE test::LLVM xacc)
