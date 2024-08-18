#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/RayBitmasks.h"
#include "ChessEngine/utils/BitBasics.h"
#include "ChessEngine/movegen/Movelist.h"

#include "ChessEngine/movegen/RayLogic.h"
#include "ChessEngine/movegen/CheckDetection.h"
#include "ChessEngine/movegen/RookGenerator.h"
#include "ChessEngine/movegen/BishopGenerator.h"
#include "ChessEngine/movegen/KnightGenerator.h"
#include "ChessEngine/movegen/QueenGenerator.h"
#include "ChessEngine/movegen/KingGenerator.h"
#include "ChessEngine/movegen/PawnGenerator.h"
#include "ChessEngine/movegen/CastlingGenerator.h"


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
        perft::SearchMemory& searchMemory
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
    perft::SearchMemory& _searchMemoryRef;
    
    CheckDetection _checkDetection;
    RookGenerator _rookGenerator;
    BishopGenerator _bishopGenerator;
    QueenGenerator _queenGenerator;
    KnightGenerator _knightGenerator;
    KingGenerator _kingGenerator;
    PawnGenerator _pawnGenerator;
    CastlingGenerator _castlingGenerator;
};

}