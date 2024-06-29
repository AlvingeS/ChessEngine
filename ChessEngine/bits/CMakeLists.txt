add_library(bits
    RayBitMasks.cpp
    KnightBitMasks.cpp
    KingBitMasks.cpp
    PawnBitMasks.cpp
    )
target_link_libraries(bits PUBLIC utils cxx_setup)