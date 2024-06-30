add_executable(game_tests
    MakeUnmakeMove_test.cpp
)

target_link_libraries(game_tests PRIVATE game masks GTest::gtest_main)
gtest_discover_tests(game_tests)