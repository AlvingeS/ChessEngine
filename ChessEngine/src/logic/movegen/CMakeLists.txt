add_library(movegen
    move_gen.cpp
    rook_gen.cpp
    bishop_gen.cpp
    queen_gen.cpp
    knight_gen.cpp
    king_gen.cpp
    pawn_gen.cpp
    castle_gen.cpp
    ray_logic.cpp
    check_detection.cpp
)

add_subdirectory(utils)

target_link_libraries(movegen PUBLIC move position attack_tables pickmove cxx_setup) # TODO: Remove dependency to pickmove!!!
