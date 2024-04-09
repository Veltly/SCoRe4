# Generate doxygen documentation

find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target(
    doc ALL
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen/
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    OPTIONAL)
endif()
