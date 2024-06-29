add_library(game 
    ChessBoard.cpp 
    MoveMaker.cpp
    ZHasher.cpp
)
target_link_libraries(game PUBLIC utils bits cxx_setup)