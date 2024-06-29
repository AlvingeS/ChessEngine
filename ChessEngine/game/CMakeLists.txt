add_library(game 
    ChessBoard.cpp 
    MoveMaker.cpp
    ZHasher.cpp
)
target_link_libraries(game PUBLIC utils masks cxx_setup)