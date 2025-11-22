install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/examples
        PATTERN "CMakeFiles" EXCLUDE
        PATTERN "*.o" EXCLUDE
        PATTERN "*.cmake" EXCLUDE
        PATTERN "Makefile" EXCLUDE
        PATTERN "build" EXCLUDE)