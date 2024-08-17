add_executable(game_tests
    Bitboards_test.cpp
    # MakeUnmakeMove_test.cpp
)

target_link_libraries(game_tests PRIVATE board masks GTest::gtest_main)
gtest_discover_tests(game_tests)