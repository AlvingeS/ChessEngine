#pragma once
#include "Move.h"
#include <vector>
#include "ChessBoard.h"
#include "ChessEngine/bits/BitMasks.h"
#include "ChessEngine/bits/BitBasics.h"

namespace game {
    class MoveGenerator {
        public:
            MoveGenerator();
            std::vector<Move>& genMoves(bool is_white);
            void genRookMoves(bool is_white);

            static constexpr int MAX_LEGAL_MOVES = 218;
        private:
            std::vector<Move> _moves; 
            ChessBoard _board;
            std::vector<std::vector<bits::U64>> _straightRayBitMasks;
            std::vector<std::vector<bits::U64>> _diagonalRayBitMasks;
            void updateGameStateBitMasks();
            bits::U64 _occupiedBitMask;
            bits::U64 _emptySquaresBitMask;
            bits::U64 _whitePiecesBitMask;
            bits::U64 _blackPiecesBitMask;
            bits::U64 _whiteSquaresBitMask;            
            bits::U64 _blackSquaresBitMask;
    };
}