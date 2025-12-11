add_library(io
    board_printer.cpp
    fen.cpp
    stockfish_perft_retriever.cpp
    utils.cpp
)

target_link_libraries(io PUBLIC movegen logic_utils position cxx_setup)