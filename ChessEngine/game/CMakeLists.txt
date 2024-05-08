add_library(game 
    ChessBoard.cpp 
    BoardPrinter.cpp
    MoveMaker.cpp
)
target_link_libraries(game PUBLIC bits cxx_setup)