add_executable(board_tests
    bitboards_test.cpp
)

target_link_libraries(board_tests PRIVATE position io test_support GTest::gtest_main)
gtest_discover_tests(board_tests)