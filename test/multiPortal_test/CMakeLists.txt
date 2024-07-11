# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/test/multiPortal_test/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/multiPortal_test/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/test/multiPortal_test/include/*.hh)

add_executable(multiPortalTest multiPortal_test.cc ${sources} ${headers})

target_link_libraries(multiPortalTest ${Geant4_LIBRARIES} surface_lib)

file(COPY ${PROJECT_SOURCE_DIR}/test/multiPortal_test/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/multiPortal_test/)
