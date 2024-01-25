#pragma once
#include "Move.h"
#include <vector>
#include "ChessBoard.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/BitBasics.h"

namespace game {
    class MoveGenerator {
        public:
            MoveGenerator();
            std::vector<Move>& genMoves(bool is_white);
            void genRookMoves(bool is_white);
            int getNumLegalMoves();
            void setBoardFromFen(std::string fen);

            std::vector<Move>& getMoves() {
                return _moves;
            }

            static constexpr int MAX_LEGAL_MOVES = 218;
        private:
            // Member variables
            std::vector<Move> _moves; 
            ChessBoard _board;

            // Masks
            std::vector<bits::StraightRays> _straightRayBitmasks;
            std::vector<bits::DiagonalRays> _diagonalRayBitmasks;
            bits::U64 _occupiedBitmask;
            bits::U64 _emptySquaresBitmask;
            bits::U64 _whitePiecesBitmask;
            bits::U64 _blackPiecesBitmask;
            bits::U64 _whiteSquaresBitmask;            
            bits::U64 _blackSquaresBitmask;

            // Member functions
            void updateGameStateBitmasks();
            void addMove(int bitIndexFrom, int bitIndexTo, PieceType pieceType);
            void addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom, PieceType pieceType);
            void addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, PieceType pieceType);
    };
}