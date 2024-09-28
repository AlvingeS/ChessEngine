add_executable(move_tests
    MakeUnmakeMove_test.cpp
)

target_link_libraries(move_tests PRIVATE movegen move GTest::gtest_main)
gtest_discover_tests(move_tests)