add_library(movegen
    move_gen.cpp
    rook_gen.cpp
    bishop_gen.cpp
    queen_gen.cpp
    knight_gen.cpp
    king_gen.cpp
    pawn_gen.cpp
    castle_gen.cpp
    rays.cpp
    check_detection.cpp
    containers.cpp
)

target_link_libraries(movegen PUBLIC move position logic_utils attack_tables pickmove cxx_setup) # TODO: Remove dependency to pickmove!!!
