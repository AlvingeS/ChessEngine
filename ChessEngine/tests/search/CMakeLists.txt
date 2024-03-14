add_executable(search_tests
    perft.cpp
)

target_link_libraries(search_tests PRIVATE search GTest::gtest_main)
gtest_discover_tests(search_tests)