add_library(masks
    RayBitMasks.cpp
    KnightBitMasks.cpp
    KingBitMasks.cpp
    PawnBitMasks.cpp
    )
target_link_libraries(masks PUBLIC utils cxx_setup)