# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/test/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/test/include/*.hh)

add_executable(SurfaceTest surface_testsetup.cc ${sources} ${headers})

target_link_libraries(SurfaceTest ${Geant4_LIBRARIES})

file(COPY ${PROJECT_SOURCE_DIR}/test/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/)
