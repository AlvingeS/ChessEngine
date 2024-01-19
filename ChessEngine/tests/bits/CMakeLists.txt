add_executable(bits_tests
    BitBasics.cpp
)

target_link_libraries(bits_tests PRIVATE bits GTest::gtest_main)
gtest_discover_tests(bits_tests)