#include "MoveGenerator.h"
#include "ChessEngine/bits/BitMasks.h"
#include "ChessEngine/bits/BitUtils.h"

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
    }
}