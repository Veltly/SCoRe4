# Find Geant4
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

# Collect sources and headers
file(GLOB_RECURSE score4_sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE score4_headers ${PROJECT_SOURCE_DIR}/src/*.hh)

# Build library
add_library(score4 ${score4_sources})

target_include_directories(
        score4 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(score4 ${Geant4_LIBRARIES})

# ----------------------------------------------------------------------------
# Install library and headers
# ----------------------------------------------------------------------------

# Install the library
install(
        TARGETS score4
        EXPORT Score4Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/score4)

# Install the full header tree
install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/score4
        FILES_MATCHING
        PATTERN "*.hh")
