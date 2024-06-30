#pragma once

#include "ChessEngine/common.h"


#include "ChessEngine/utils/BitBasics.h"
#include "ChessEngine/board/GameStateBitMasks.h"
#include "ChessEngine/move/Move.h"

#include "CommonLogic.h"

namespace movegen {
    class RayLogic {
        public:
            // Constructor
            RayLogic(board::GameStateBitMasks& gameStateBitmasks, int& moveIndex, CommonLogic* commonLogic);

            // Public member functions
            void getMovesFromStraightRay(U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<move::Move>& moveList);
            void getMovesFromDiagonalRay(U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<move::Move>& moveList);
            bool checkStraightRay(U64& straightRay, bool firstBlockerOnLSB, U64& opponentRooksAndQueens);
            bool checkDiagonalRay(U64& diagonalRay, bool firstBlockerOnLSB, U64& opponentBishopsAndQueens);
        private:

            // Private member functions
            void addMovesFromFreeRay(U64 freeRay, int bitIndexFrom, std::vector<move::Move>& moveList);
            void addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, std::vector<move::Move>& moveList);
            void addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex, bool alongFile, bool startFromBlocker, int rookRank, int rookFile, int bitIndexFrom, std::vector<move::Move>& moveList);
            void addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, int bishopRank, int bishopFile, int bitIndexFrom, std::vector<move::Move>& moveList);

            // Memory
            std::vector<int> _freeRayIndices;
            
            // Private member variables
            board::GameStateBitMasks& _gameStateBitMasks;
            int& _moveIndex;
            CommonLogic* _commonLogic;
    };
}