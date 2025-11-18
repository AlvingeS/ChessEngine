add_library(io
    board_printer.cpp
    fen.cpp
    stockfish_perft_retriever.cpp
)

target_link_libraries(io PUBLIC movegen position cxx_setup)