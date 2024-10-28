#include "ChessEngine/src/io/Fen.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace io {

void setBoardFromFen(
    const std::string& fen, 
    model::Bitboards& bitboards, 
    model::GameStateBitmasks& gameStateBitmasks, 
    model::SquaresLookup& squaresLookup) 
{
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
            model::PieceType type = model::CharToPieceType(c);
            bitboards.setPieceTypeBit(rank * 8 + 7 - file, type);
            file++;
        }
    }

    squaresLookup.fillSquaresLookupFromBitboards(bitboards);
    gameStateBitmasks.fillBlackPiecesBitmaskFromBitboards(bitboards);
    gameStateBitmasks.fillWhitePiecesBitmaskFromBitboards(bitboards);
    gameStateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

std::string getFenFromBoard(const model::SquaresLookup& squaresLookup)
{
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

        model::PieceType type = squaresLookup.getPieceTypeAtIndex(i);
        if (type == model::PieceType::EMPTY) {
            emptyCount++;
        } else {
            if (emptyCount != 0) {
                fen += std::to_string(emptyCount);
                emptyCount = 0;
            }
            fen += model::pieceTypeToChar(type);
        }
    }

    return fen;
}

} // namespace io