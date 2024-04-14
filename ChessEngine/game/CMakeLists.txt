add_library(game 
    ChessBoard.cpp 
    BoardPrinter.cpp 
)
target_link_libraries(game PUBLIC bits cxx_setup)