add_executable(utils_test
    BitBasics_test.cpp
    ChessUtils_test.cpp
)

target_link_libraries(utils_test PRIVATE utils GTest::gtest_main)
gtest_discover_tests(utils_test)