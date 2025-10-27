# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(../example_surface/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/example_surface/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/test/example_surface/include/*.hh)

add_executable(example_surface example_surface.cc ${sources} ${headers})

target_link_libraries(example_surface ${Geant4_LIBRARIES} surface)

file(COPY ../example_surface/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/example_surface/)
