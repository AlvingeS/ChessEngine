add_library(logic_utils
    utils.cpp
)

target_link_libraries(logic_utils PUBLIC move position cxx_setup) # TODO: Remove dependency to pickmove!!!


add_subdirectory(eval)
add_subdirectory(makemove)
add_subdirectory(movegen)
add_subdirectory(zobrist)
add_subdirectory(attack_tables)