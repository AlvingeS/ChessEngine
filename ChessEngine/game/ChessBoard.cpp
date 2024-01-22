#include "ChessBoard.h"

namespace game {

    ChessBoard::ChessBoard() {
        initPieceBitboards();
    }

    void ChessBoard::initPieceBitboards() {
        _bitboards[PieceType::W_PAWN] = 0x000000000000FF00ULL;
        _bitboards[PieceType::W_KNIGHT] = 0x0000000000000042ULL;
        _bitboards[PieceType::W_BISHOP] = 0x0000000000000024ULL;
        _bitboards[PieceType::W_ROOK] = 0x0000000000000081ULL;
        _bitboards[PieceType::W_QUEEN] = 0x0000000000000010ULL;
        _bitboards[PieceType::W_KING] = 0x0000000000000008ULL;

        _bitboards[PieceType::B_PAWN] = 0x00FF000000000000ULL;
        _bitboards[PieceType::B_KNIGHT] = 0x4200000000000000ULL;
        _bitboards[PieceType::B_BISHOP] = 0x2400000000000000ULL;
        _bitboards[PieceType::B_ROOK] = 0x8100000000000000ULL;
        _bitboards[PieceType::B_QUEEN] = 0x1000000000000000ULL;
        _bitboards[PieceType::B_KING] = 0x0800000000000000ULL;
    }

    bits::U64 ChessBoard::getWhitePiecesBitMask() {
        bits::U64 w_all = 0;
        for (int i = 0; i < 6; i++) {
            w_all |= _bitboards[static_cast<PieceType>(i)];
        }

        return w_all;
    }

    bits::U64 ChessBoard::getBlackPiecesBitMask() {
        bits::U64 b_all = 0;
        for (int i = 6; i < 12; i++) {
            b_all |= _bitboards[static_cast<PieceType>(i)];
        }

        return b_all;
    }

    void ChessBoard::setBoardFromFen(const std::string& fen) {
        for (auto& board : _bitboards) {
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
                _bitboards[type] |= bit;
                file++;
            }
        }
    }
}