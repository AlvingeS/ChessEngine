add_executable(game_tests
    bitboard_init.cpp
)

target_link_libraries(game_tests PRIVATE game GTest::gtest_main)
gtest_discover_tests(game_tests)