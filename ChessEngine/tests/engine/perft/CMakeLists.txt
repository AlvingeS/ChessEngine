add_executable(perft_test
    perftStartPos_test.cpp
    perftPosTwo_test.cpp
    perftPosThree_test.cpp
    perftPosFive_test.cpp
)

target_link_libraries(perft_test PRIVATE perft board GTest::gtest_main)
gtest_discover_tests(perft_test)