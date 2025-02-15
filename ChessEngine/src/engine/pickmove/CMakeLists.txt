add_library(pickmove
    MovePicker.cpp
    SearchMemory.cpp
)
target_link_libraries(pickmove PUBLIC io eval makemove movegen cxx_setup)