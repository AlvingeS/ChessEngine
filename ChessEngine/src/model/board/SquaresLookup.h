#pragma once

#include "ChessEngine/src/model/board/PieceType.h"
#include "ChessEngine/src/model/board/Bitboards.h"

namespace model {

class SquaresLookup {

public:
    /// @brief Constructor
    SquaresLookup(const Bitboards& bitboards);

    /// @brief Get the piece type at the given index
    inline PieceType getPieceTypeAtIndex(int index) const
    {
        return _squaresLookup[index];
    }

    /// @brief Set the piece type at the given index
    inline void setPieceTypeAtIndex(int index, PieceType pieceType)
    {
        _squaresLookup[index] = pieceType;
    }

    /// @brief Fill the squares lookup from the given bitboards
    void fillSquaresLookupFromBitboards(const Bitboards& bitboards);
    
private:

    std::vector<PieceType> _squaresLookup;
};

} // namespace model