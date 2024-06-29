#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/bits/BitBasics.h"
#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/Move.h"

#include "CommonLogic.h"

namespace movegen {
    class RayLogic {
        public:
            // Constructor
            RayLogic(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic);

            // Public member functions
            void getMovesFromStraightRay(U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<game::Move>& moveList);
            void getMovesFromDiagonalRay(U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<game::Move>& moveList);
            bool checkStraightRay(U64& straightRay, bool firstBlockerOnLSB, U64& opponentRooksAndQueens);
            bool checkDiagonalRay(U64& diagonalRay, bool firstBlockerOnLSB, U64& opponentBishopsAndQueens);
        private:

            // Private member functions
            void addMovesFromFreeRay(U64 freeRay, int bitIndexFrom, std::vector<game::Move>& moveList);
            void addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, std::vector<game::Move>& moveList);
            void addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex, bool alongFile, bool startFromBlocker, int rookRank, int rookFile, int bitIndexFrom, std::vector<game::Move>& moveList);
            void addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, int bishopRank, int bishopFile, int bitIndexFrom, std::vector<game::Move>& moveList);

            // Memory
            std::vector<int> _freeRayIndices;
            
            // Private member variables
            game::ChessBoard& _board;
            int& _moveIndex;
            CommonLogic* _commonLogic;
    };
}