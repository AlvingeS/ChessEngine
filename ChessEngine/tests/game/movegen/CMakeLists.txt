add_executable(movegen_tests
    RookGenerator_test.cpp
    BishopGenerator_test.cpp
    QueenGenerator_test.cpp
    KnightGenerator_test.cpp
    KingGenerator_test.cpp
    PawnGenerator_test.cpp
    CastlingGenerator_test.cpp
    CheckDetection_test.cpp
)

target_link_libraries(movegen_tests PRIVATE move perft movegen board utils GTest::gtest_main)
gtest_discover_tests(movegen_tests)