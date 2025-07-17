# ----------------------------------------------------------------------------
# Setup the project
#

# Prior to version 3.3 the find_package(doxygen) feature was broken
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(
  Surface
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
     "${CMAKE_SOURCE_DIR}/compile_commands.json" SYMBOLIC)

# Use GNU-style hierarchy for installing build products
include(GNUInstallDirs)

# Define the C++ Standard to use
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER clang++)

option(DOC "Create doxygen documentation" ON)
option(COMPILE_TEST "Compile tests which also function as examples" OFF)

add_subdirectory(src)

if(DOC)
  include(cmake/FindDoxygen.cmake)
endif()

if(COMPILE_TEST)
  add_subdirectory(test)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/mac/ DESTINATION mac)
