add_library(move
    MoveMaker.cpp
    MoveRetractor.cpp
    MoveUtils.cpp
)
target_link_libraries(move PUBLIC board perft cxx_setup)