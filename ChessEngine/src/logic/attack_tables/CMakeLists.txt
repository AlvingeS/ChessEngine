add_library(attack_tables
    attack_tables.cpp
    king_attack_table.cpp
    knight_attack_table.cpp
    pawn_attack_table.cpp
    line_ray_attack_table.cpp
    diag_ray_attack_table.cpp
)

target_link_libraries(attack_tables PUBLIC cxx_setup)