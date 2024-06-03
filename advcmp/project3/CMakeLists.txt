cmake_minimum_required(VERSION 3.1)
project(AdvancedCompilers C CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

set(LLVM_INSTALL_DIR "" CACHE PATH "LLVM installation directory")
set(LLVM_CMAKE_CONFIG_DIR "${LLVM_INSTALL_DIR}/lib/cmake/llvm/")
list(APPEND CMAKE_PREFIX_PATH "${LLVM_CMAKE_CONFIG_DIR}")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
    -fdiagnostics-color=always")

if (NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_subdirectory(lib)
