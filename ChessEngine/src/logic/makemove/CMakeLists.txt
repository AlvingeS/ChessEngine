add_library(makemove
    move_maker.cpp
    move_retractor.cpp
)
target_link_libraries(makemove PUBLIC position move logic_utils zobrist cxx_setup)