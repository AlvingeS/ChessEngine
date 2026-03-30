add_library(pickmove
    move_picker.cpp
    time_manager.cpp
    transposition_table.cpp
)
target_link_libraries(pickmove PUBLIC io eval makemove movegen uci cxx_setup Threads::Threads)