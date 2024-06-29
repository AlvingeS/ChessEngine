add_library(utils
    BoardPrinter.cpp
    BitBasics.cpp
)

target_link_libraries(utils PUBLIC cxx_setup)