add_executable(position_test
    bitboards_test.cpp
)

target_link_libraries(position_test PRIVATE position io test_support GTest::gtest_main)
gtest_discover_tests(position_test)