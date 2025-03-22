add_library(io
    BoardPrinter.cpp
    Fen.cpp
)

target_link_libraries(io PUBLIC movegen board cxx_setup)