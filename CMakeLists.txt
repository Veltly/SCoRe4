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

# Define a default build type when using a single-mode tool like make/ninja If
# you're using a build tool that supports multiple modes (Visual Studio, Xcode),
# this setting has no effect. It can be overriden by passing
# ``-DCMAKE_BUILD_TYPE=<type>`` when invoking CMake
if(NOT CMAKE_CONFIGURATION_TYPES)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        RelWithDebInfo
        CACHE
          STRING
          "Choose the type of build, options are: None Release MinSizeRel Debug RelWithDebInfo"
          FORCE)
  else()
    set(CMAKE_BUILD_TYPE
        "${CMAKE_BUILD_TYPE}"
        CACHE
          STRING
          "Choose the type of build, options are: None Release MinSizeRel Debug RelWithDebInfo"
          FORCE)
  endif()
endif()

# Define the C++ Standard to use
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# If GCC is used add compiler options ...
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # ... for all configurations
  add_compile_options(-Wall -Wextra -Wpedantic -pthread -fPIC
                      --param=max-vartrack-size=90000000)
  # ... only for debug
  add_compile_options($<$<CONFIG:Debug>:-g3>)
endif()

# --- Declare options
# -----------------------------------------------------------
option(DOC "Whether or not to create doxygen documentation." OFF)

# --- enable unit testing capabilities
# ------------------------------------------
include(CTest)

# --- target for Doxygen documentation
# ------------------------------------------
if(DOC)
  include(cmake/FindDoxygen.cmake)
endif()

# ----------------------------------------------------------------------------
# find Geant4 packages
#
find_package(Geant4 10.6.3)
# ----------------------------------------------------------------------------
# Setup Geant4  include directories and compile definitions
#
include(${Geant4_USE_FILE})

# ----------------------------------------------------------------------------
# Locate sources and headers for this project
#

# Geant4 and ROOT headers
include_directories(${Geant4_INCLUDE_DIR})

# Geant4 defintions
add_definitions(${Geant4_DEFINITIONS})

# For public headers
include_directories(${CMAKE_SOURCE_DIR}/include)
# Private header are in the same directories as the cc-files
include_directories(${CMAKE_SOURCE_DIR}/src)

set(PROJECT_MAIN_LIST Surface.cc)
foreach(PROJECT_MAIN ${PROJECT_MAIN_LIST})
  string(REPLACE ".cc" "" appName ${PROJECT_MAIN})
  message(${appName})
  message(${PROJECT_MAIN})
  if("${PROJECT_MAIN}" MATCHES "Surface")
    file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/ *.cc)
    file(GLOB_RECURSE headers ${CMAKE_SOURCE_DIR}/include/ *.hh)
    add_executable(${appName} ${CMAKE_SOURCE_DIR}/${PROJECT_MAIN} ${sources}
                              ${headers})
    # # Make sure YourLib is linked to each app
    target_link_libraries(${appName} ${Geant4_LIBRARIES} uuid)
    # Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
    install(TARGETS ${appName} DESTINATION bin)
    # ----------------------------------------------------------------------------
    # Install the examples, macros, spectra and geometry object files
    #
  else()
    file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/detectorResponse/*.cc)
    file(GLOB_RECURSE headers ${CMAKE_SOURCE_DIR}/include/detectorResponse/*.hh)
    add_executable(${appName} ${CMAKE_SOURCE_DIR}/${PROJECT_MAIN} ${sources})
    target_link_libraries(${appName} ${ROOT_LIBRARIES} ${EVTLIB_NAME} config++
                          stdc++ uuid)
    install(TARGETS ${appName} DESTINATION bin)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/detConfig/ DESTINATION detConfig)
  endif()
endforeach(PROJECT_MAIN ${PROJECT_MAIN_LIST})
