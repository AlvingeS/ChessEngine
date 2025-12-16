add_executable(mass_fen_test
    depth_one_make_gen_hash_test.cpp
)

target_link_libraries(mass_fen_test PRIVATE makemove zobrist movegen GTest::gtest_main)
gtest_discover_tests(mass_fen_test)