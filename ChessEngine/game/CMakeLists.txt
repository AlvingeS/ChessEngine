add_library(game ChessBoard.cpp BoardPrinter.cpp MoveGenerator.cpp)
target_link_libraries(game PUBLIC cxx_setup)