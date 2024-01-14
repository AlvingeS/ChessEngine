add_executable(search_tests
    dummy_test.cpp
)

target_link_libraries(search_tests PRIVATE search GTest::gtest_main)
gtest_discover_tests(search_tests)