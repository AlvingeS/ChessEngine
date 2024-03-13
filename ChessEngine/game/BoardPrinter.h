#pragma once
#include <vector>
#include <unordered_map>
#include "PieceType.h"
#include "ChessEngine/bits/BitBasics.h"

namespace game {
    class BoardPrinter {
        public:
            // Public member functions
            BoardPrinter(std::vector<bits::U64> bitboards);
            void fillBoard(std::vector<bits::U64>  bitboards);
            void printBoard();

        private:
            // Private member variables
            std::vector<std::vector<char>> _board;

            // Private member functions
            bool isValidPiece(PieceType piece_type);
    };
}