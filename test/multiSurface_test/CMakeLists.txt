# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/test/multiSurface_test/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/multiSurface_test/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/test/multiSurface_test/include/*.hh)

add_executable(multiSurfaceTest multiSurface_test.cc ${sources} ${headers})

target_link_libraries(multiSurfaceTest ${Geant4_LIBRARIES} surface_lib)

file(COPY ${PROJECT_SOURCE_DIR}/test/multiSurface_test/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/multiSurface_test/)
