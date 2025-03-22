#pragma once
#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/board/PieceType.h"

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

#include "ChessEngine/src/model/board/PieceMap.inl"