cmake_minimum_required(VERSION 3.5)
project(example_surface)

# Find Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Sources and headers
file(GLOB sources src/*.cc)
file(GLOB headers include/*.hh)

add_executable(example_surface example_surface.cc ${sources}
        ${headers})

# Example-local includes
target_include_directories(example_surface
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link with main library and Geant4
target_link_libraries(
        example_surface
        PRIVATE score4
        PRIVATE ${Geant4_LIBRARIES})

# Define compile-time macro for installed macro path
target_compile_definitions(
        example_surface
        PRIVATE
        INSTALL_MACRO_DIR="${CMAKE_INSTALL_PREFIX}/share/example_surface/macros"
)

# Install executable
install(TARGETS example_surface RUNTIME DESTINATION bin)

# Install macros
install(DIRECTORY macros/ DESTINATION share/example_surface/macros)

# Optional: install headers if the example defines its own types
install(DIRECTORY include/ DESTINATION share/example_surface/include)
