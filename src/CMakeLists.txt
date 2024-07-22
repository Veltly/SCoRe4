find_package(Geant4 REQUIRED)

include(${Geant4_USE_FILE})

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/src/*.hh)

add_library(surface ${sources} ${headers})

set_target_properties(surface PROPERTIES PUBLIC_HEADER "${headers}")

target_include_directories(surface PUBLIC ${PROJECT_SOURCE_DIR}/src/)

target_link_libraries(surface ${Geant4_LIBRARIES})

install(TARGETS surface LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/Portal
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/Service
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ParticleGenerator
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/SurfaceGenerator
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
