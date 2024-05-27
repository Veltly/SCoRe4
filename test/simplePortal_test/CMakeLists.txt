# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/test/simplePortal_test/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/simplePortal_test/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/test/simplePortal_test/include/*.hh)

add_executable(SimplePortalTest simplePortal_test.cc ${sources} ${headers})

target_link_libraries(SimplePortalTest ${Geant4_LIBRARIES} surface_lib)

file(COPY ${PROJECT_SOURCE_DIR}/test/simplePortal_test/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/simplePortal_test/)
