# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/test/periodicPortal_test/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/periodicPortal_test/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/test/periodicPortal_test/include/*.hh)

add_executable(periodicPortalTest periodicPortal_test.cc ${sources} ${headers})

target_link_libraries(periodicPortalTest ${Geant4_LIBRARIES} surface)

file(COPY ${PROJECT_SOURCE_DIR}/test/periodicPortal_test/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/periodicPortal_test/)
