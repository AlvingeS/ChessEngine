add_library(zobrist
    z_hasher.cpp
)

target_link_libraries(zobrist PUBLIC position logic_utils cxx_setup)
