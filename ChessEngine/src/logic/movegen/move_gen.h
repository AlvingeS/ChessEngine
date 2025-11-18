#pragma once

#include "logic/movegen/pawn_gen.h"
#include "logic/movegen/bishop_gen.h"
#include "logic/movegen/knight_gen.h"
#include "logic/movegen/castling_gen.h"
#include "logic/movegen/rook_gen.h"
#include "logic/movegen/queen_gen.h"
#include "logic/movegen/king_gen.h"
#include "logic/movegen/check_detection.h"
#include "logic/movegen/utils/bit_basics.h"

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