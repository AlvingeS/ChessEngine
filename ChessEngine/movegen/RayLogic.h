#pragma once

#include "ChessEngine/common.h"


#include "ChessEngine/utils/BitBasics.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/move/Move.h"

#include "CommonLogic.h"

namespace movegen {

class RayLogic {

public:
    // Constructor
    RayLogic(
        board::GameStateBitmasks& gameStateBitmasks,
        int& moveIndex,
        CommonLogic* commonLogic
    );

    // Public member functions
    void getMovesFromStraightRay(
        bitmask ray,
        bool blockerOnLSB,
        bool alongFile,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        std::vector<move::Move>& moveList
    );

    void getMovesFromDiagonalRay(
        bitmask ray,
        bool blockerOnLSB,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        std::vector<move::Move>& moveList
    );

    bool checkStraightRay(
        bitmask& straightRay,
        bool firstBlockerOnLSB,
        bitmask& opponentRooksAndQueens
    );

    bool checkDiagonalRay(
        bitmask& diagonalRay,
        bool firstBlockerOnLSB,
        bitmask& opponentBishopsAndQueens
    );

private:
    // Private member functions
    void addMovesFromFreeRay(
        bitmask freeRay,
        int bitIndexFrom,
        std::vector<move::Move>& moveList
    );

    void addMoveIfBlockerIsEnemy(
        int blockerIndex,
        bool isWhite,
        int bitIndexFrom,
        std::vector<move::Move>& moveList
    );

    void addMovesBetweenBlockerAndPieceOnStraightRay(
        int blockerIndex,
        bool alongFile,
        bool startFromBlocker,
        int rookRank,
        int rookFile,
        int bitIndexFrom,
        std::vector<move::Move>& moveList
    );

    void addMovesBetweenBlockerAndPieceOnDiagonalRay(
        int blockerIndex,
        bool startFromBlocker,
        int bishopRank,
        int bishopFile,
        int bitIndexFrom,
        std::vector<move::Move>& moveList
    );

    // Memory
    std::vector<int> _freeRayIndices;
    
    // Private member variables
    board::GameStateBitmasks& _gameStateBitmasks;
    int& _moveIndex;
    CommonLogic* _commonLogic;
};

}