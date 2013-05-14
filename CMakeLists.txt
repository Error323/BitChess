# === Set the project name and specify the required CMAKE version.
project(BitChess CXX C)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# === Compiler options
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wno-long-long -Wno-variadic-macros")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

option(ENABLE_TESTS "Enable/disable test framework" ON)
option(ENABLE_PROFILING "Enable/disable profiling" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif(NOT CMAKE_BUILD_TYPE)

if(ENABLE_PROFILING)
  set(CMAKE_BUILD_TYPE "Release")
  add_definitions(-DENABLE_PROFILING)
endif(ENABLE_PROFILING)

# === Project version
execute_process(
  COMMAND git describe --tags --always
  OUTPUT_VARIABLE VERSION
  ERROR_VARIABLE GIT_ERROR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/version.h.in
  ${PROJECT_BINARY_DIR}/version.h
)

# === Include sources.
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)
include_directories(${PROJECT_BINARY_DIR})

# === Set cmake 3rd library modules path
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# === Include directories
include_directories(${CPPUNIT_INCLUDE_DIR})

# === Add all tests
if(ENABLE_TESTS)
  enable_testing()
  find_package(CppUnit REQUIRED)
  foreach(TEST ${TESTS})
    add_executable(${TEST} ${${TEST}_SOURCES})
    target_link_libraries(${TEST} ${CPPUNIT_LIBRARIES})
    add_test(${TEST} ${TEST})
  endforeach(TEST)
endif(ENABLE_TESTS)

# === Create the emulator binary.
add_executable(${PROJECT_NAME} ${SOURCES})

# === Status report
message(STATUS "")
message(STATUS " C++:")
message(STATUS "   C++ Compiler:         ${CMAKE_CXX_COMPILER}")
message(STATUS "   C++ (Release):        ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "   C++ (Debug):          ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "   C++ (RelWithDebInfo): ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "")
message(STATUS " CMakeflags (${CMAKE_PROJECT_NAME} v${VERSION}):")
message(STATUS "   CMAKE_BUILD_TYPE      ${CMAKE_BUILD_TYPE}")
message(STATUS "   ENABLE_TESTS          ${ENABLE_TESTS}")
message(STATUS "   ENABLE_PROFILING      ${ENABLE_PROFILING}")
message(STATUS "")
