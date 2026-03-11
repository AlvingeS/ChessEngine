add_executable(perft_test
    perft_test.cpp
)

target_link_libraries(perft_test PRIVATE perft position test_support attack_tables GTest::gtest_main)
gtest_discover_tests(perft_test)