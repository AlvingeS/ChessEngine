add_executable(search_tests
    perftStartPos.cpp
    perftPosTwo.cpp
    perftPosThree.cpp
    perftPosFive.cpp
)

target_link_libraries(search_tests PRIVATE search GTest::gtest_main)
gtest_discover_tests(search_tests)