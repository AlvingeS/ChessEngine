#include "ChessEngine/src/model/board/PieceMap.h"

#include "ChessEngine/src/model/board/Bitboards.h"

namespace model {

PieceMap::PieceMap(const Bitboards& bitboards) 
{
    fillPieceMapFromBitboards(bitboards);
}

void PieceMap::fillPieceMapFromBitboards(const Bitboards& bitboards) 
{
    _pieceMap = std::vector<Piece::Type>(64, Piece::Type::EMPTY);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 12; j++) {
            if (bitboards.getBitboardFromIndex(j) & (1ULL << i)) {
                _pieceMap[i] = Piece::getTypeFromInt(j);
                break;
            }
        }
    }
}

} // namespace model