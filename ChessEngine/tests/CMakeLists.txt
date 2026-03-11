if (BUILD_TESTING)
    include(GoogleTest)

    add_library(test_support INTERFACE)
    target_include_directories(test_support INTERFACE
        ${PROJECT_SOURCE_DIR}/ChessEngine
    )

    add_subdirectory(mass_fen)
    add_subdirectory(perft)
endif()