add_executable(attack_tables_tests)

target_sources(attack_tables_tests PRIVATE
    king_attack_table_test.cpp
    knight_attack_table_test.cpp
    pawn_attack_table_test.cpp
    ray_attack_table_test.cpp
)

target_link_libraries(attack_tables_tests PRIVATE attack_tables test_support GTest::gtest_main)

gtest_discover_tests(attack_tables_tests)
