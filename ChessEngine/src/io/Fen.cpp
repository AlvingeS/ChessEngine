#include "ChessEngine/src/io/Fen.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace io {

void Fen::setBoardFromFen(
    const std::string& fen, 
    model::Bitboards& bitboards, 
    model::StateBitmasks& stateBitmasks, 
    model::PieceMap& pieceMap) 
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
            model::Piece::Type type = model::Piece::getTypeFromChar(c);
            bitboards.setPieceTypeBit(rank * 8 + 7 - file, type);
            file++;
        }
    }

    pieceMap.fillPieceMapFromBitboards(bitboards);
    stateBitmasks.fillBlackPiecesBitmaskFromBitboards(bitboards);
    stateBitmasks.fillWhitePiecesBitmaskFromBitboards(bitboards);
    stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

std::string Fen::getFenFromBoard(const model::PieceMap& pieceMap)
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

        model::Piece::Type type = pieceMap.getPieceTypeAtIndex(i);
        if (type == model::Piece::Type::EMPTY) {
            emptyCount++;
        } else {
            if (emptyCount != 0) {
                fen += std::to_string(emptyCount);
                emptyCount = 0;
            }
            fen += model::Piece::getCharFromType(type);
        }
    }

    return fen;
}

} // namespace io