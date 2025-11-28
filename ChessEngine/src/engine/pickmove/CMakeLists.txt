add_library(pickmove
    move_picker.cpp
)
target_link_libraries(pickmove PUBLIC io eval makemove movegen cxx_setup)