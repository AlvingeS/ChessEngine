add_executable(game_tests
    ChessBoard_test.cpp
    MoveGenerator_Rooks_test.cpp
    MoveGenerator_Knights_test.cpp
)

target_link_libraries(game_tests PRIVATE game bits GTest::gtest_main)
gtest_discover_tests(game_tests)