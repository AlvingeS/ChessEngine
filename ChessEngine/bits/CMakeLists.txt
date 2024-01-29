add_library(bits
    RayBitMasks.cpp
    ChessUtils.cpp
    KnightBitMasks.cpp
    KingBitMasks.cpp
    PawnBitMasks.cpp
    )
target_link_libraries(bits PUBLIC cxx_setup)