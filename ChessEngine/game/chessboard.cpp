#include "headers/chessboard.h"

namespace game {

    ChessBoard::ChessBoard() {
        init_piece_bitboards();
        init_macro_bitboards();
    }

    void ChessBoard::init_piece_bitboards() {
        m_bitboards[PieceType::W_PAWN] = 0x000000000000FF00ULL;
        m_bitboards[PieceType::W_KNIGHT] = 0x0000000000000042ULL;
        m_bitboards[PieceType::W_BISHOP] = 0x0000000000000024ULL;
        m_bitboards[PieceType::W_ROOK] = 0x0000000000000081ULL;
        m_bitboards[PieceType::W_QUEEN] = 0x0000000000000010ULL;
        m_bitboards[PieceType::W_KING] = 0x0000000000000008ULL;

        m_bitboards[PieceType::B_PAWN] = 0x00FF000000000000ULL;
        m_bitboards[PieceType::B_KNIGHT] = 0x4200000000000000ULL;
        m_bitboards[PieceType::B_BISHOP] = 0x2400000000000000ULL;
        m_bitboards[PieceType::B_ROOK] = 0x8100000000000000ULL;
        m_bitboards[PieceType::B_QUEEN] = 0x1000000000000000ULL;
        m_bitboards[PieceType::B_KING] = 0x0800000000000000ULL;
    }

    void ChessBoard::init_macro_bitboards() {
        m_bitboards[PieceType::W_ALL] = gen_w_all_bitboard();
        m_bitboards[PieceType::B_ALL] = gen_b_all_bitboard();
        m_bitboards[PieceType::OCCUPIED] = gen_occupied_bitboard();
        m_bitboards[PieceType::EMPTY] = gen_empty_bitboard();
    }

    utils::U64 ChessBoard::gen_w_all_bitboard() {
        utils::U64 w_all = 0;
        for (int i = 0; i < 6; i++) {
            w_all = utils::or_bits(w_all, m_bitboards[static_cast<PieceType>(i)]);
        }

        return w_all;
    }

    utils::U64 ChessBoard::gen_b_all_bitboard() {
        utils::U64 b_all = 0;
        for (int i = 6; i < 12; i++) {
            b_all = utils::or_bits(b_all, m_bitboards[static_cast<PieceType>(i)]);
        }

        return b_all;
    }

    utils::U64 ChessBoard::gen_occupied_bitboard() {
        return utils::or_bits(m_bitboards[PieceType::W_ALL], m_bitboards[PieceType::B_ALL]);
    }

    utils::U64 ChessBoard::gen_empty_bitboard() {
        return utils::not_bits(m_bitboards[PieceType::OCCUPIED]);
    }

    void ChessBoard::set_board_from_fen(const std::string& fen) {
        for (auto& board : m_bitboards) {
            board.second = 0ULL;
        }

        int rank = 7;
        int file = 0;
        for (char c : fen) {
            if (c == ' ') {
                break;
            } else if (c == '/') {
                rank--;
                file = 0;
            } else if (std::isdigit(c)) {
                file += c - '0';
            } else {
                PieceType type = char_to_piece_type(c);
                uint64_t bit = 1ULL << (rank * 8 + (7 - file));
                m_bitboards[type] |= bit;
                file++;
            }
        }
    }
}