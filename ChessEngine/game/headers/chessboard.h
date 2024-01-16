#pragma once
#include <unordered_map>
#include "ChessEngine/game/headers/piecetype.h"
#include "ChessEngine/utils/headers/bit_logic.h"

namespace game {
    class ChessBoard {
        public:
            ChessBoard();

            utils::U64 get_bitboard(PieceType piece_type) {
                return m_bitboards[piece_type];
            }

            std::unordered_map<PieceType, utils::U64> get_bitboards() {
                return m_bitboards;
            }

            private:
                std::unordered_map<PieceType, utils::U64> m_bitboards;

                void init_piece_bitboards();
                void init_macro_bitboards();
                utils::U64 gen_w_all_bitboard();
                utils::U64 gen_b_all_bitboard();
                utils::U64 gen_occupied_bitboard();
                utils::U64 gen_empty_bitboard();

                bool w_to_move;
    };
}