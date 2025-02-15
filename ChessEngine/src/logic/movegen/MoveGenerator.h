#pragma once

#include "ChessEngine/src/logic/movegen/PawnGenerator.h"
#include "ChessEngine/src/logic/movegen/BishopGenerator.h"
#include "ChessEngine/src/logic/movegen/KnightGenerator.h"
#include "ChessEngine/src/logic/movegen/CastlingGenerator.h"
#include "ChessEngine/src/logic/movegen/RookGenerator.h"
#include "ChessEngine/src/logic/movegen/QueenGenerator.h"
#include "ChessEngine/src/logic/movegen/KingGenerator.h"
#include "ChessEngine/src/logic/movegen/CheckDetection.h"

#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace model {
    class Board;
    class Bitboards;
    class StateBitmasks;
    class Movelist;
}

namespace engine {
    class SearchMemory;
}

namespace logic {

class MoveMaker;
class MoveRetractor;

class MoveGenerator {

public:
    MoveGenerator(
        model::Board& board,
        logic::MoveMaker& moveMaker,
        logic::MoveRetractor& moveRetractor
    );

    void genMoves(
        bool isWhite,
        model::Movelist& moveList,
        bitmask enpessantTarget,
        unsigned char castlingRight
    );
    
    void genRookMoves(bool isWhite, model::Movelist& moveList);
    void genBishopMoves(bool isWhite, model::Movelist& moveList);
    void genQueenMoves(bool isWhite, model::Movelist& moveList);
    void genKnightMoves(bool isWite, model::Movelist& moveList);
    void genKingMoves(bool isWhite, model::Movelist& moveList);
    
    void genPawnMoves(
        bool isWhite,
        model::Movelist& moveList,
        bitmask enpessantTarget
    );

    void genCastlingMoves(
        bool isWhite,
        model::Movelist& moveList,
        unsigned char castlingRights
    );
    
    bool isInCheck(bool isWhite);

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    
    logic::MoveMaker& _moveMaker;
    logic::MoveRetractor& _moveRetractor;
    
    CheckDetection _checkDetection;
    RookGenerator _rookGenerator;
    BishopGenerator _bishopGenerator;
    QueenGenerator _queenGenerator;
    KnightGenerator _knightGenerator;
    KingGenerator _kingGenerator;
    PawnGenerator _pawnGenerator;
    CastlingGenerator _castlingGenerator;
};

} // namespace logic