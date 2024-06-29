add_executable(game_tests
    ChessBoard_test.cpp
    # MakeUnmakeMove_test.cpp
)

target_link_libraries(game_tests PRIVATE game masks GTest::gtest_main)
gtest_discover_tests(game_tests)