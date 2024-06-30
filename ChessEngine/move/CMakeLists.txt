add_library(move
    MoveMaker.cpp
    BitBoardUpdater.cpp
    BitMaskUpdater.cpp
    LookupUpdater.cpp
)
target_link_libraries(move PUBLIC cxx_setup)