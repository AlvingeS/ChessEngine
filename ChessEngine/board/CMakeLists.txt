add_library(board 
    BitBoards.cpp
    GameStateBitMasks.cpp
    SquaresLookup.cpp
    ZHasher.cpp
    PieceType.cpp
)
target_link_libraries(board PUBLIC utils masks cxx_setup)