add_library(move
    Move.cpp
    Movelist.cpp
)
target_link_libraries(move PUBLIC board cxx_setup)