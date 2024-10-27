#pragma once

#include "ChessEngine/src/representation/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/CheckDetection.h"

#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"

#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/logic/makemove/MoveMaker.h"
#include "ChessEngine/src/logic/makemove/MoveRetractor.h"

namespace logic {
namespace movegen {

constexpr bitmask whiteKingSideCastleMask = 0x000000000000006ULL;
constexpr bitmask whiteQueenSideCastleMask = 0x0000000000000070ULL;
constexpr bitmask blackKingSideCastleMask = 0x0600000000000000ULL;
constexpr bitmask blackQueenSideCastleMask = 0x7000000000000000ULL;

class CastlingGenerator {

public:
    CastlingGenerator(
        const board::Bitboards& bitboards,
        const board::GameStateBitmasks& gameStateBitmasks,
        move::MoveMaker& moveMaker,
        move::MoveRetractor& moveRetractor,
        CheckDetection* checkDetection
    );
    
    void generate(
        bool isWhite, 
        Movelist& movelist, 
        unsigned char castlingRights
    );

private:
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;
    
    move::MoveMaker& _moveMakerRef;
    move::MoveRetractor& _moveRetractorRef;
    
    CheckDetection* _checkDetection;

    bitmask _whiteKingSideCastleBitmask;
    bitmask _whiteQueenSideCastleBitmask;
    bitmask _blackKingSideCastleBitmask;
    bitmask _blackQueenSideCastleBitmask;

    void genSingleCastleMove(
        bool isWhite,
        bool isKingSide,
        Movelist& movelist
    );

    bool kingAndRookOnCastlingSquares(bool isWhite, bool isKingSide) const;
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);
    void unmakeTemporaryKingMove(bool isWhite, bool isKingSide);
};

} // namespace movegen
} // namespace logic