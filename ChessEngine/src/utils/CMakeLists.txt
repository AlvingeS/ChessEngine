add_library(utils
    BoardPrinter.cpp
    BitBasics.cpp
    Fen.cpp
    Containers.cpp
)

target_link_libraries(utils PUBLIC board cxx_setup)