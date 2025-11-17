add_library(io
    BoardPrinter.cpp
    Fen.cpp
    StockfishPerftRetriever.cpp
)

target_link_libraries(io PUBLIC movegen board cxx_setup)