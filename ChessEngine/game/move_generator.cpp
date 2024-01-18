#include "headers/move_generator.h"
#include "ChessEngine/utils/headers/bit_masks.h"

namespace game {
    MoveGenerator::MoveGenerator() {
        m_moves = std::vector<Move>(MAX_LEGAL_MOVES);
        m_board = ChessBoard();
    }

    std::vector<Move>& MoveGenerator::generate_moves(bool is_white) {
        generate_rook_moves(is_white);

        return m_moves;
    }

    void MoveGenerator::generate_rook_moves(bool is_white) {
        std::vector<int> indices;
        utils::U64 iso_rook_bitboard;
        std::vector<utils::U64> ray_bitmasks;
        std::vector<bool> ray_blocked;

        if (is_white) {
            indices = utils::get_bit_indices(m_board.get_bitboard(PieceType::W_ROOK));
        } else {
            indices = utils::get_bit_indices(m_board.get_bitboard(PieceType::B_ROOK));
        }

        for (int index : indices) {
            iso_rook_bitboard = 1ULL << index;

            ray_bitmasks = utils::create_straight_ray_bitmasks(index);


            // Loop through each ray and check if it is blocked
            for (utils::U64 ray : ray_bitmasks) {
                if (ray & m_board.get_bitboard(PieceType::OCCUPIED) != 0) {
                    ray_blocked.push_back(true);
                } else {
                    ray_blocked.push_back(false);
                }
            }


        }
    }
}