# === Set the project name and specify the required CMAKE version.
project(BitChess CXX C)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# === Compiler options
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DDEBUG")

# === Project version
execute_process(
  COMMAND git describe --tags
  OUTPUT_VARIABLE GIT_TAG
  ERROR_VARIABLE GIT_ERROR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(GIT_TAG)
  set(VERSION ${GIT_TAG})
else(NOT GIT_TAG)
  execute_process(
    COMMAND git describe --always
    OUTPUT_VARIABLE GIT_SHA1
    ERROR_VARIABLE GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(VERSION ${GIT_SHA1})
endif(GIT_TAG)

configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/version.h.in
  ${PROJECT_BINARY_DIR}/version.h
)

# === Include sources.
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)
include_directories(${PROJECT_BINARY_DIR})

# === Set cmake 3rd library modules path
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# === Find Dependencies.
find_package(CppUnit REQUIRED)

# === Include directories
include_directories(${CPPUNIT_INCLUDE_DIR})

# === Add all tests
enable_testing()
foreach(TEST ${TESTS})
  add_executable(${TEST} ${${TEST}_SOURCES})
  target_link_libraries(${TEST} ${CPPUNIT_LIBRARIES})
  add_test(${TEST} ${TEST})
endforeach(TEST)


# === Create the emulator binary.
add_executable(${PROJECT_NAME} ${SOURCES})
