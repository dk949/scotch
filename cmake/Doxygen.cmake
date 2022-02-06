function (enable_doxygen)
    option(ENABLE_DOXYGEN "Enable doxygen doc builds of source" ON)
    if (ENABLE_DOXYGEN)
        # Note: If you want to change some Doxyfile setting, prepend `DOXYGEN_` to the sdetting name and set it as a
        # cmake variable

        set(DOXYFILE_SUFFIX doxygen-docs)

        set(DOXYGEN_ENABLE_PREPROCESSING YES)
        set(DOXYGEN_EXTRACT_ALL YES)
        set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
        set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")
        set(DOXYGEN_SEARCH_INCLUDES YES)
        set(DOXYGEN_EXTRACT_PRIVATE YES)
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")

        find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)
        if (DOXYGEN_FOUND)
            message(STATUS "Found dot, enabling graphs")
            set(DOXYGEN_CALLER_GRAPH
                YES
                CACHE STRING "generate caller graph"
            )
            set(DOXYGEN_CALL_GRAPH
                YES
                CACHE STRING "generate call graph"
            )
        endif ()
        doxygen_add_docs(${DOXYFILE_SUFFIX} ${PROJECT_SOURCE_DIR})

    endif ()
endfunction ()
