#pragma once
#include <vector>
#include <unordered_map>
#include "PieceType.h"
#include "ChessEngine/bits/headers/BitLogic.h"

namespace game {
    class BoardPrinter {
        public:
            BoardPrinter(std::unordered_map<PieceType, bits::U64> bitboards);
            void fillBoard(std::unordered_map<PieceType, bits::U64> bitboards);
            void printBoard();
        private:
            std::vector<std::vector<char>> _board;
            bool isValidPiece(PieceType piece_type);
    };
}