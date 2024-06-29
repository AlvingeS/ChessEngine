#include "ChessBoard.h"

namespace game {

    ChessBoard::ChessBoard() {
        _bitboards.reserve(12);
        _noCaptureOrPawnMoveCount = 0;
        initPieceBitboards();
        fillWhitePiecesBitmask();
        fillBlackPiecesBitmask();
        fillOccupiedPiecesBitmask();
        fillEmptySquaresBitmask();
    }

    void ChessBoard::initPieceBitboards() {        
        _whitePawnsBitboard = 0x000000000000FF00ULL;
        _whiteKnightsBitboard = 0x0000000000000042ULL;
        _whiteBishopsBitboard = 0x0000000000000024ULL;
        _whiteRooksBitboard = 0x0000000000000081ULL;
        _whiteQueensBitboard = 0x0000000000000010ULL;
        _whiteKingBitboard = 0x0000000000000008ULL;

        _blackPawnsBitboard = 0x00FF000000000000ULL;
        _blackKnightsBitboard = 0x4200000000000000ULL;
        _blackBishopsBitboard = 0x2400000000000000ULL;
        _blackRooksBitboard = 0x8100000000000000ULL;
        _blackQueensBitboard = 0x1000000000000000ULL;
        _blackKingBitboard = 0x0800000000000000ULL;

        _bitboards.push_back(&_whitePawnsBitboard);
        _bitboards.push_back(&_whiteKnightsBitboard);
        _bitboards.push_back(&_whiteBishopsBitboard);
        _bitboards.push_back(&_whiteRooksBitboard);
        _bitboards.push_back(&_whiteQueensBitboard);
        _bitboards.push_back(&_whiteKingBitboard);

        _bitboards.push_back(&_blackPawnsBitboard);
        _bitboards.push_back(&_blackKnightsBitboard);
        _bitboards.push_back(&_blackBishopsBitboard);
        _bitboards.push_back(&_blackRooksBitboard);
        _bitboards.push_back(&_blackQueensBitboard);
        _bitboards.push_back(&_blackKingBitboard);

        _enPessantTarget = 0ULL;

        // Sets piecetype lookup to zero vector 64 squares
        _squaresLookup = std::vector<PieceType>(64, PieceType::EMPTY);
        fillSquaresLookup();
    }

    bool ChessBoard::isDeadPosition() {
        // It is assumed in this method that the kings may never dissapear
        int popCount = bits::popCount(_occupiedPiecesBitmask);

        if (popCount == 2) {
            return true;
        }

        if (popCount == 3) {
            if (_whiteBishopsBitboard != 0 || _blackBishopsBitboard != 0) {
                return true;
            }

            if (_whiteKnightsBitboard != 0 || _blackKnightsBitboard != 0) {
                return true;
            }
        }

        return false;
    }

    void ChessBoard::fillSquaresLookup() {
        _squaresLookup = std::vector<PieceType>(64, PieceType::EMPTY);

        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 12; j++) {
                if (*(_bitboards[j]) & (1ULL << i)) {
                    _squaresLookup[i] = intToPieceType(j);
                }
            }
        }
    }

    void ChessBoard::fillWhitePiecesBitmask() {
        U64 w_all = 0;
        for (int i = 0; i < 6; i++) {
            w_all |= *(_bitboards[i]);
        }

        _whitePiecesBitmask = w_all;
    }

    void ChessBoard::fillBlackPiecesBitmask() {
        U64 b_all = 0;
        for (int i = 6; i < 12; i++) {
            b_all |= *(_bitboards[i]);
        }

        _blackPiecesBitmask = b_all;
    }

    void ChessBoard::setBoardFromFen(const std::string& fen) {
        for (int i = 0; i < 12; i++) {
            *(_bitboards[i]) = 0ULL;
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
                PieceType type = CharToPieceType(c);
                uint64_t bit = 1ULL << (rank * 8 + (7 - file));
                *(_bitboards[pieceTypeToInt(type)]) |= bit;
                file++;
            }
        }

        fillSquaresLookup();
        fillBlackPiecesBitmask();
        fillWhitePiecesBitmask();
        fillOccupiedPiecesBitmask();
        fillEmptySquaresBitmask();
    }

    std::string ChessBoard::getFenFromBoard() {
        std::string fen = "";
        int emptyCount = 0;

        for (int i = 63; i >= 0; i--) {
            if ((i + 1) % 8 == 0 && i != 63) {
                if (emptyCount != 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "/";
            }

            PieceType type = _squaresLookup[i];
            if (type == PieceType::EMPTY) {
                emptyCount++;
            } else {
                if (emptyCount != 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += pieceTypeToChar(type);
            }
        }

        return fen;
    }
}