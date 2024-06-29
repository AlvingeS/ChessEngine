add_library(perft
    Searcher.cpp
    SearchMemory.cpp
)
target_link_libraries(perft PUBLIC movegen game evaluation utils cxx_setup)