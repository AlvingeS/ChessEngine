add_library(game 
    ChessBoard.cpp 
    BoardPrinter.cpp 
    MoveGenerator.cpp
    Move.cpp    
)
target_link_libraries(game PUBLIC cxx_setup)