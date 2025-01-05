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
#include "ChessEngine/src/model/move/Movelist.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

class MoveGenerator {

public:
    // Public  er variables
    static constexpr int MAX_LEGAL_MOVES = 218;

    // Public member functions
    MoveGenerator(
        const model::Bitboards& bitboards,
        const model::StateBitmasks& stateBitmasks,
        logic::MoveMaker& moveMaker,
        logic::MoveRetractor& moveRetractor
    );

    void genMoves(
        bool isWhite,
        model::Movelist& moveList,
        int currentDepth,
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
        int currentDepth,
        engine::SearchMemory& searchMemory
    );

    void genCastlingMoves(
        bool isWhite,
        model::Movelist& moveList,
        unsigned char castlingRights
    );
    
    bool isInCheck(bool isWhite);

private:
    // Private member variables
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    
    logic::MoveMaker& _moveMaker;
    logic::MoveRetractor& _moveRetractor;
    engine::SearchMemory& _searchMemory;
    
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