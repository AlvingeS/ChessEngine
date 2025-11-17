#pragma once
#include "common.h"

#include "model/position/PieceType.h"

namespace model {

class Bitboards;

class PieceMap {

public:
    PieceMap(const Bitboards& bitboards);

    Piece::Type getPieceTypeAtIndex(int index) const;
    
    void setPieceTypeAtIndex(int index, Piece::Type pieceType);
    void fillPieceMapFromBitboards(const Bitboards& bitboards);
    
private:
    std::vector<Piece::Type> _pieceMap;
};

} // namespace model

#include "model/position/PieceMap.inl"