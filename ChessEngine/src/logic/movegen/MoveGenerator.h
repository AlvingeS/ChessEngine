#pragma once

#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/CheckDetection.h"
#include "ChessEngine/src/logic/movegen/RookGenerator.h"
#include "ChessEngine/src/logic/movegen/BishopGenerator.h"
#include "ChessEngine/src/logic/movegen/KnightGenerator.h"
#include "ChessEngine/src/logic/movegen/QueenGenerator.h"
#include "ChessEngine/src/logic/movegen/KingGenerator.h"
#include "ChessEngine/src/logic/movegen/PawnGenerator.h"
#include "ChessEngine/src/logic/movegen/CastlingGenerator.h"
#include "ChessEngine/src/representation/move/Movelist.h"

#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {
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
} // namespace logic