#include "headers/MoveGenerator.h"
#include "ChessEngine/bits/headers/BitMasks.h"

namespace game {
    MoveGenerator::MoveGenerator() {
        _moves = std::vector<Move>(MAX_LEGAL_MOVES);
        _board = ChessBoard();
    }

    std::vector<Move>& MoveGenerator::genMoves(bool is_white) {
        genRookMoves(is_white);

        return _moves;
    }

    void MoveGenerator::genRookMoves(bool is_white) {
        std::vector<int> indices;
        bits::U64 iso_rook_bitboard;
        std::vector<bits::U64> ray_bitmasks;
        std::vector<bool> ray_blocked;

        if (is_white) {
            indices = bits::getBitIndices(_board.getBitboard(PieceType::W_ROOK));
        } else {
            indices = bits::getBitIndices(_board.getBitboard(PieceType::B_ROOK));
        }

        for (int index : indices) {
            iso_rook_bitboard = 1ULL << index;

            ray_bitmasks = bits::create_straight_ray_bitmasks(index);


            // Loop through each ray and check if it is blocked
            for (bits::U64 ray : ray_bitmasks) {
                if (ray & _board.getBitboard(PieceType::OCCUPIED) != 0) {
                    ray_blocked.push_back(true);
                } else {
                    ray_blocked.push_back(false);

                    // Loop through each square in the ray and add a move for each
                    
                }
            }


        }
    }
}