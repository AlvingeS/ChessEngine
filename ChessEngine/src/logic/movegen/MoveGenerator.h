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
        const representation::board::Bitboards& bitboards,
        const representation::board::GameStateBitmasks& gameStateBitmasks,
        logic::makemove::MoveMaker& moveMaker,
        logic::makemove::MoveRetractor& moveRetractor
    );

    void genMoves(
        bool isWhite,
        representation::move::Movelist& moveListRef,
        int currentDepth,
        unsigned char castlingRight
    );
    
    void genRookMoves(bool isWhite, representation::move::Movelist& moveListRef);
    void genBishopMoves(bool isWhite, representation::move::Movelist& moveListRef);
    void genQueenMoves(bool isWhite, representation::move::Movelist& moveListRef);
    void genKnightMoves(bool isWite, representation::move::Movelist& moveListRef);
    void genKingMoves(bool isWhite, representation::move::Movelist& moveListRef);
    
    void genPawnMoves(
        bool isWhite,
        representation::move::Movelist& moveListRef,
        int currentDepth,
        engine::search::SearchMemory& searchMemory
    );

    void genCastlingMoves(
        bool isWhite,
        representation::move::Movelist& moveListRef,
        unsigned char castlingRights
    );
    
    bool isInCheck(bool isWhite);

private:
    // Private member variables
    const representation::board::Bitboards& _bitboardsRef;
    const representation::board::GameStateBitmasks& _gameStateBitmasksRef;
    
    logic::makemove::MoveMaker& _moveMakerRef;
    logic::makemove::MoveRetractor& _moveRetractorRef;
    engine::search::SearchMemory& _searchMemoryRef;
    
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