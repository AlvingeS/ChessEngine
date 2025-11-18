add_executable(perft_test
    perft_start_pos_test.cpp
    perft_pos_two_test.cpp
    perft_pos_three_test.cpp
    perft_pos_five_test.cpp
)

target_link_libraries(perft_test PRIVATE perft position test_support GTest::gtest_main)
gtest_discover_tests(perft_test)