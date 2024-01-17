#include "headers/move_generator.h"


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
        
    }
}