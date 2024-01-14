if (BUILD_TESTING)
    add_executable(unit_tests tools.cpp)
    target_link_libraries(unit_tests PRIVATE tools GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()