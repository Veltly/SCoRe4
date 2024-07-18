# Testing with a testsetup

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/test/periodicPortalBig_test/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/test/periodicPortalBig_test/src/*.cc)
file(GLOB headers
     ${PROJECT_SOURCE_DIR}/test/periodicPortalBig_test/include/*.hh)

add_executable(periodicPortalBigTest periodicPortalBig_test.cc ${sources}
                                     ${headers})

target_link_libraries(periodicPortalBigTest ${Geant4_LIBRARIES} surface)

file(COPY ${PROJECT_SOURCE_DIR}/test/periodicPortalBig_test/macros
     DESTINATION ${PROJECT_BINARY_DIR}/test/periodicPortalBig_test/)
