add_library(position 
    bitboards.cpp
    state_bitmasks.cpp
    piece_map.cpp
    z_hasher.cpp
    piece_type.cpp
)
target_link_libraries(position PUBLIC cxx_setup)