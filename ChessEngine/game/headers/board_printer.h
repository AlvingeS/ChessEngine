#pragma once
#include <vector>
#include <unordered_map>
#include "piecetype.h"
#include "ChessEngine/utils/headers/bit_logic.h"

namespace game {
    class BoardPrinter {
        public:
            BoardPrinter(std::unordered_map<PieceType, utils::U64> bitboards);
            void fill_board(std::unordered_map<PieceType, utils::U64> bitboards);
            void print_board();
        private:
            std::vector<std::vector<char>> m_board;
            bool is_piece_bitboard(PieceType piece_type);
    };
}