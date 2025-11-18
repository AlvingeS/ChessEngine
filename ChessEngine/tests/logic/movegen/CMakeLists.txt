add_executable(movegen_tests)

# Add test subdirectories
add_subdirectory(utils)
add_subdirectory(bitmasks)

target_sources(movegen_tests PRIVATE
    rook_gen_test.cpp
    bishop_gen_test.cpp
    queen_gen_test.cpp
    knight_gen_test.cpp
    king_gen_test.cpp
    pawn_gen_test.cpp
    castling_gen_test.cpp
    check_detection_test.cpp
)

target_link_libraries(movegen_tests PRIVATE makemove perft movegen position test_support GTest::gtest_main)

gtest_discover_tests(movegen_tests)
