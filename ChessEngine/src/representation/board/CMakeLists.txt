add_library(board 
    Bitboards.cpp
    GameStateBitmasks.cpp
    SquaresLookup.cpp
    ZHasher.cpp
    PieceType.cpp
)
target_link_libraries(board PUBLIC cxx_setup)