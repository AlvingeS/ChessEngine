#pragma once
#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace model {

class Bitboards;

class PieceMap {

public:
    PieceMap(const Bitboards& bitboards);

    PieceType getPieceTypeAtIndex(int index) const;
    
    void setPieceTypeAtIndex(int index, PieceType pieceType);
    void fillPieceMapFromBitboards(const Bitboards& bitboards);
    
private:
    std::vector<PieceType> _pieceMap;
};

} // namespace model

#include "ChessEngine/src/model/board/PieceMap.inl"