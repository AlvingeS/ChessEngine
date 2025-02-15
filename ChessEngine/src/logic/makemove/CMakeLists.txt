add_library(makemove
    MoveMaker.cpp
    MoveRetractor.cpp
    MoveUtils.cpp
)
target_link_libraries(makemove PUBLIC board move pickmove cxx_setup)