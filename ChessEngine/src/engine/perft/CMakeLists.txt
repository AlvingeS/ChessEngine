add_library(perft
    perft.cpp
    perftData.cpp
)
target_link_libraries(perft PUBLIC movegen evaluation makemove cxx_setup)