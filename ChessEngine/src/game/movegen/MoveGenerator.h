#pragma once

#include "ChessEngine/src/game/movegen/RayLogic.h"
#include "ChessEngine/src/game/movegen/CheckDetection.h"
#include "ChessEngine/src/game/movegen/RookGenerator.h"
#include "ChessEngine/src/game/movegen/BishopGenerator.h"
#include "ChessEngine/src/game/movegen/KnightGenerator.h"
#include "ChessEngine/src/game/movegen/QueenGenerator.h"
#include "ChessEngine/src/game/movegen/KingGenerator.h"
#include "ChessEngine/src/game/movegen/PawnGenerator.h"
#include "ChessEngine/src/game/movegen/CastlingGenerator.h"
#include "ChessEngine/src/game/movegen/Movelist.h"

#include "ChessEngine/src/game/move/Move.h"
#include "ChessEngine/src/game/masks/RayBitmasks.h"

#include "ChessEngine/src/utils/BitBasics.h"

namespace game {
namespace movegen {

class MoveGenerator {

public:
    // Public  er variables
    static constexpr int MAX_LEGAL_MOVES = 218;

    // Public member functions
    MoveGenerator(
        const board::Bitboards& bitboards,
        const board::GameStateBitmasks& gameStateBitmasks,
        move::MoveMaker& moveMaker,
        move::MoveRetractor& moveRetractor
    );

    void genMoves(
        bool isWhite,
        Movelist& moveListRef,
        int currentDepth,
        unsigned char castlingRight
    );
    
    void genRookMoves(bool isWhite, Movelist& moveListRef);
    void genBishopMoves(bool isWhite, Movelist& moveListRef);
    void genQueenMoves(bool isWhite, Movelist& moveListRef);
    void genKnightMoves(bool isWite, Movelist& moveListRef);
    void genKingMoves(bool isWhite, Movelist& moveListRef);
    
    void genPawnMoves(
        bool isWhite,
        Movelist& moveListRef,
        int currentDepth,
        ponder::perft::SearchMemory& searchMemory
    );

    void genCastlingMoves(
        bool isWhite,
        Movelist& moveListRef,
        unsigned char castlingRights
    );
    
    bool isInCheck(bool isWhite);

private:
    // Private member variables
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;
    
    move::MoveMaker& _moveMakerRef;
    move::MoveRetractor& _moveRetractorRef;
    ponder::perft::SearchMemory& _searchMemoryRef;
    
    CheckDetection _checkDetection;
    RookGenerator _rookGenerator;
    BishopGenerator _bishopGenerator;
    QueenGenerator _queenGenerator;
    KnightGenerator _knightGenerator;
    KingGenerator _kingGenerator;
    PawnGenerator _pawnGenerator;
    CastlingGenerator _castlingGenerator;
};

} // namespace movegen
} // namespace game