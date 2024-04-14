#pragma once
#include <vector>
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
            void getMovesFromStraightRay(bits::U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<game::Move>& moveList);
            void getMovesFromDiagonalRay(bits::U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<game::Move>& moveList);
            bool checkStraightRay(bits::U64& straightRay, bool firstBlockerOnLSB, bits::U64& opponentRooksAndQueens);
            bool checkDiagonalRay(bits::U64& diagonalRay, bool firstBlockerOnLSB, bits::U64& opponentBishopsAndQueens);
        private:

            // Private member functions
            void addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom, std::vector<game::Move>& moveList);
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