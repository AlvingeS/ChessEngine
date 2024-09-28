add_library(masks
    RayBitmasks.cpp
    KnightBitmasks.cpp
    KingBitmasks.cpp
    PawnBitmasks.cpp
    )
target_link_libraries(masks PUBLIC utils cxx_setup)