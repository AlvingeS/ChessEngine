#pragma once
#include "ChessEngine/src/common.h"

namespace model {
    class Move;
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class MoveMaker;
class MoveRetractor;
class CheckDetection;

class CastlingGenerator {
    
public:
    CastlingGenerator(
        model::Board& board,
        logic::MoveMaker& moveMaker,
        logic::MoveRetractor& moveRetractor,
        CheckDetection* checkDetection
    );
    
    void generate(
        bool isWhite, 
        model::Movelist& movelist, 
        unsigned char castlingRights
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    
    logic::MoveMaker& _moveMaker;
    logic::MoveRetractor& _moveRetractor;
    
    CheckDetection* _checkDetection;

    bitmask _whiteKingSideCastleBitmask;
    bitmask _whiteQueenSideCastleBitmask;
    bitmask _blackKingSideCastleBitmask;
    bitmask _blackQueenSideCastleBitmask;

    void genSingleCastleMove(
        bool isWhite,
        bool isKingSide,
        model::Movelist& movelist
    );

    bool kingAndRookOnCastlingSquares(bool isWhite, bool isKingSide) const;
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);
    void unmakeTemporaryKingMove(bool isWhite, bool isKingSide);

    const bitmask mWhiteKingSideCastleMask = 0x000000000000006ULL;
    const bitmask mWhiteQueenSideCastleMask = 0x0000000000000070ULL;
    const bitmask mBlackKingSideCastleMask = 0x0600000000000000ULL;
    const bitmask mBlackQueenSideCastleMask = 0x7000000000000000ULL;
};

} // namespace logic