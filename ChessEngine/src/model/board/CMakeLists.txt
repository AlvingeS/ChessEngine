add_library(board 
    Bitboards.cpp
    StateBitmasks.cpp
    PieceMap.cpp
    ZHasher.cpp
    PieceType.cpp
)
target_link_libraries(board PUBLIC cxx_setup)