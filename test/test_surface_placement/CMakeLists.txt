# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(../test_surface_placement/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/test_surface_placement/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/test/test_surface_placement/include/*.hh)

add_executable(test_surface_placement test_surface_placement.cc ${sources} ${headers})

target_link_libraries(test_surface_placement ${Geant4_LIBRARIES} surface)

file(COPY ../test_surface_placement/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/test_surface_placement/)
