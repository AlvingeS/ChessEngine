add_executable(game_tests
    ChessBoard_test.cpp
    MoveGenerator_Rooks_test.cpp
    MoveGenerator_Knights_test.cpp
    MoveGenerator_Bishops_test.cpp
    MoveGenerator_Kings_test.cpp
    MoveGenerator_Queens_test.cpp
    MoveGenerator_Pawns_test.cpp
    MoveGenerator_MoveCounts_test.cpp
    MoveGenerator_Castling_test.cpp
    MoveGenerator_CheckDetection_test.cpp
    MakeUnmakeMove_test.cpp
)

target_link_libraries(game_tests PRIVATE game bits GTest::gtest_main)
gtest_discover_tests(game_tests)