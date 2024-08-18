#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"
#include "Bitboards.h"

namespace board {

class SquaresLookup {

public:
    SquaresLookup(const Bitboards& bitboards);

    inline PieceType getPieceTypeAtIndex(int index) const
    {
        return _squaresLookup[index];
    }

    inline void setPieceTypeAtIndex(int index, PieceType pieceType)
    {
        _squaresLookup[index] = pieceType;
    }

    void fillSquaresLookupFromBitboards(const Bitboards& bitboards);
    
private:

    std::vector<PieceType> _squaresLookup;
};

} // namespace board