add_executable(evaluation_tests
    dummy_test.cpp
)

target_link_libraries(evaluation_tests PRIVATE evaluation GTest::gtest_main)
gtest_discover_tests(evaluation_tests)