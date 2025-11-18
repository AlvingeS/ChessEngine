add_library(makemove
    move_maker.cpp
    move_retractor.cpp
    move_utils.cpp
)
target_link_libraries(makemove PUBLIC position move cxx_setup)