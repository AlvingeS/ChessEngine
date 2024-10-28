add_library(io
    BoardPrinter.cpp
    Fen.cpp
)

target_link_libraries(io PUBLIC evaluation makemove movegen cxx_setup)