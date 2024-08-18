add_library(move
    MoveMaker.cpp
    MoveRetractor.cpp
    MoveUtils.cpp
)
target_link_libraries(move PUBLIC cxx_setup)