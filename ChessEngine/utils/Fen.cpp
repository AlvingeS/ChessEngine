#include "Fen.h"

#include "ChessEngine/game/PieceType.h"

namespace utils {
    void setBoardFromFen(const std::string& fen, game::BitBoards& bitboards, game::GameStateBitMasks& gameStateBitmasks, game::SquaresLookup& squaresLookup) {
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
                game::PieceType type = game::CharToPieceType(c);
                uint64_t bit = 1ULL << (rank * 8 + (7 - file));
                bitboards.getBitboardFromIndex(game::pieceTypeToInt(type)) |= bit;
                file++;
            }
        }

        squaresLookup.fillSquaresLookup(bitboards);
        gameStateBitmasks.fillBlackPiecesBitmask(bitboards);
        gameStateBitmasks.fillWhitePiecesBitmask(bitboards);
        gameStateBitmasks.fillOccupiedPiecesBitmask();
        gameStateBitmasks.fillEmptySquaresBitmask();
    }

    std::string& getFenFromBoard(game::SquaresLookup& squaresLookup) {
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

            game::PieceType type = squaresLookup.getPieceTypeAtIndex(i);
            if (type == game::PieceType::EMPTY) {
                emptyCount++;
            } else {
                if (emptyCount != 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += game::pieceTypeToChar(type);
            }
        }

        return fen;
    }
}