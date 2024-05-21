find_package(Geant4 REQUIRED)

include(${Geant4_USE_FILE})

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/src/*.hh)

add_library(surface_lib ${sources} ${headers})

target_include_directories(surface_lib PUBLIC ${PROJECT_SOURCE_DIR}/src/)

target_link_libraries(surface_lib ${Geant4_LIBRARIES})
