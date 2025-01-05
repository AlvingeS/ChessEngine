#pragma once
#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace model {

class Bitboards;

class PieceMap {

public:
    /// @brief Constructor
    PieceMap(const Bitboards& bitboards);

    /// @brief Get the piece type at the given index
    inline PieceType getPieceTypeAtIndex(int index) const
    {
        return _pieceMap[index];
    }

    /// @brief Set the piece type at the given index
    inline void setPieceTypeAtIndex(int index, PieceType pieceType)
    {
        _pieceMap[index] = pieceType;
    }

    /// @brief Fill the squares lookup from the given bitboards
    void fillPieceMapFromBitboards(const Bitboards& bitboards);
    
private:

    std::vector<PieceType> _pieceMap;
};

} // namespace model