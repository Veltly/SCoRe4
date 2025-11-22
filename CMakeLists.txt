# ----------------------------------------------------------------------------
# Setup the project
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(
        Score4
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json" SYMBOLIC)

# Use GNU-style hierarchy: bin/, lib/, include/, share/
include(GNUInstallDirs)

# ----------------------------------------------------------------------------
# C++ standard
# ----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER clang++)

# ----------------------------------------------------------------------------
# Options
# ----------------------------------------------------------------------------
option(DOC "Create doxygen documentation" ON)
option(EXAMPLES "Install examples" ON)
option(COMPILE_TEST "Compile tests" OFF)

# ----------------------------------------------------------------------------
# Build library in src/
# ----------------------------------------------------------------------------
add_subdirectory(src)

# ----------------------------------------------------------------------------
# Documentation
# ----------------------------------------------------------------------------
if(DOC)
  include(cmake/FindDoxygen.cmake)
endif()

# ----------------------------------------------------------------------------
# Examples
# ----------------------------------------------------------------------------
if(EXAMPLES)
  add_subdirectory(examples)
endif()

# ----------------------------------------------------------------------------
# Tests
# ----------------------------------------------------------------------------
if(COMPILE_TEST)
  add_subdirectory(test)
endif()

# ----------------------------------------------------------------------------
# Install resources
# ----------------------------------------------------------------------------
install(DIRECTORY ${PROJECT_SOURCE_DIR}/mac/
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/mac)
