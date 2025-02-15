add_library(movegen)

add_subdirectory(utils)
add_subdirectory(bitmasks)

target_sources(movegen PRIVATE
    MoveGenerator.cpp
    RookGenerator.cpp
    BishopGenerator.cpp
    QueenGenerator.cpp
    KnightGenerator.cpp
    KingGenerator.cpp
    PawnGenerator.cpp
    CastlingGenerator.cpp
    RayLogic.cpp
    CheckDetection.cpp
)

target_link_libraries(movegen PUBLIC pickmove move board cxx_setup)
