cmake_minimum_required(VERSION 3.22)

project(qiree LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Note: C language is necessary for XACC and LLVM "try_compile" statements
enable_language(C)

# XACC is required
find_package(XACC REQUIRED)

# Find LLVM and the components we require.
find_package(LLVM REQUIRED)
if(NOT LLVM_VERSION EQUAL 16)
  message(WARNING "This test expects LLVM version 16")
endif()

llvm_map_components_to_libnames(llvm_libs
  Core
  ExecutionEngine
  Interpreter
  MC
  MCJIT
  Support
  nativecodegen
)

# Add library for core files
add_library(qiree
  src/GlobalMappingAssociator.cpp
  src/ExecutionBackendHandle.cpp
  src/ExecutionBackendFactory.cpp
  src/XACCExecutionBackend.cpp
)
target_compile_features(qiree
  PUBLIC
    cxx_std_17
)
target_link_libraries(qiree
  PUBLIC
    ${llvm_libs}
    XACC::xacc
)
target_include_directories(qiree
  PUBLIC
    "${LLVM_INCLUDE_DIRS}"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Create executable
add_executable(QuantumExecutionEngine
  src/QuantumExecutionEngine.cpp 
)
target_link_libraries(QuantumExecutionEngine qiree)