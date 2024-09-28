add_executable(board_tests
    Bitboards_test.cpp
    # MakeUnmakeMove_test.cpp
)

target_link_libraries(board_tests PRIVATE board utils GTest::gtest_main)
gtest_discover_tests(board_tests)