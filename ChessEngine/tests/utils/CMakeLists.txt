add_executable(utils_tests
    bit_logic.cpp
)

target_link_libraries(utils_tests PRIVATE utils GTest::gtest_main)
gtest_discover_tests(utils_tests)