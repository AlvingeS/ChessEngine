add_library(perft
    perft.cpp
    perft_data.cpp
)
target_link_libraries(perft PUBLIC movegen eval io makemove cxx_setup)