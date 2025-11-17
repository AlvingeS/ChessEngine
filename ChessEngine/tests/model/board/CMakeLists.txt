add_executable(board_tests
    Bitboards_test.cpp
)

target_link_libraries(board_tests PRIVATE board io test_support GTest::gtest_main)
gtest_discover_tests(board_tests)