add_library(movegen
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
target_link_libraries(movegen PUBLIC board masks utils cxx_setup)