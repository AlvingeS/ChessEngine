#include "Fen.h"

#include "ChessEngine/board/PieceType.h"

namespace utils {
    void setBoardFromFen(const std::string& fen, board::BitBoards& bitboards, board::GameStateBitMasks& gameStateBitmasks, board::SquaresLookup& squaresLookup) {
        bitboards.resetBitboards();

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
                board::PieceType type = board::CharToPieceType(c);
                uint64_t bit = 1ULL << (rank * 8 + (7 - file));
                bitboards.getBitboardFromIndex(board::pieceTypeToInt(type)) |= bit;
                file++;
            }
        }

        squaresLookup.fillSquaresLookup(bitboards);
        gameStateBitmasks.fillBlackPiecesBitmask(bitboards);
        gameStateBitmasks.fillWhitePiecesBitmask(bitboards);
        gameStateBitmasks.fillOccupiedPiecesBitmask();
        gameStateBitmasks.fillEmptySquaresBitmask();
    }

    std::string& getFenFromBoard(board::SquaresLookup& squaresLookup) {
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

            board::PieceType type = squaresLookup.getPieceTypeAtIndex(i);
            if (type == board::PieceType::EMPTY) {
                emptyCount++;
            } else {
                if (emptyCount != 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += board::pieceTypeToChar(type);
            }
        }

        return fen;
    }
}