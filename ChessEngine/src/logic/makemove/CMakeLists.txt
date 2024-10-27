add_library(makemove
    MoveMaker.cpp
    MoveRetractor.cpp
    MoveUtils.cpp
)
target_link_libraries(makemove PUBLIC board move search cxx_setup)