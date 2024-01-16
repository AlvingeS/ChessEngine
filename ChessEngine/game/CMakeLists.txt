add_library(game chessboard.cpp board_printer.cpp move_generator.cpp)
target_link_libraries(game PUBLIC cxx_setup)