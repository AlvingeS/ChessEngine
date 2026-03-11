add_executable(mass_fen_test
    depth_one_make_gen_hash_test.cpp
    low_depth_perft.cpp
)

target_link_libraries(mass_fen_test PRIVATE perft makemove zobrist movegen GTest::gtest_main)
gtest_discover_tests(mass_fen_test)