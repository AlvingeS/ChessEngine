#pragma once
#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace model {

class Bitboards;

class PieceMap {

public:
    PieceMap(const Bitboards& bitboards);

    inline PieceType getPieceTypeAtIndex(int index) const
    {
        return _pieceMap[index];
    }

    inline void setPieceTypeAtIndex(int index, PieceType pieceType)
    {
        _pieceMap[index] = pieceType;
    }

    void fillPieceMapFromBitboards(const Bitboards& bitboards);
    
private:

    std::vector<PieceType> _pieceMap;
};

} // namespace model