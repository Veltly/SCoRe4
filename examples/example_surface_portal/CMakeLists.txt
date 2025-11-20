# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(include)

file(GLOB sources src/*.cc)
file(GLOB headers include/*.hh)

add_executable(example_surface_portal example_surface_portal.cc ${sources} ${headers})

target_link_libraries(example_surface_portal ${Geant4_LIBRARIES} surface)

file(COPY macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/example_surface_portal/)
