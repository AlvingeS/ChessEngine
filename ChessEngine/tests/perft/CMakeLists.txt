add_executable(perft_test
    perftStartPos.cpp
    perftPosTwo.cpp
    perftPosThree.cpp
    perftPosFive.cpp
)

target_link_libraries(perft_test PRIVATE perft GTest::gtest_main)
gtest_discover_tests(perft_test)