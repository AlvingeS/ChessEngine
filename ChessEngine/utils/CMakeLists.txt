add_library(utils
    BoardPrinter.cpp
    ChessUtils.cpp
)

target_link_libraries(utils PUBLIC cxx_setup)