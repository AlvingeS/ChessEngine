add_executable(bits_tests
    BitBasics_test.cpp
    BitMasks_test.cpp
    BitUtils_test.cpp
)

target_link_libraries(bits_tests PRIVATE bits GTest::gtest_main)
gtest_discover_tests(bits_tests)