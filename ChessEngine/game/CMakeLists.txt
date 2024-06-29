add_library(game 
    BitBoards.cpp
    GameStateBitMasks.cpp
    SquaresLookup.cpp
    MoveMaker.cpp
    ZHasher.cpp
)
target_link_libraries(game PUBLIC utils masks cxx_setup)