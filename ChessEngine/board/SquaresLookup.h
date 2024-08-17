#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"
#include "Bitboards.h"

namespace board {

class SquaresLookup {

public:
    // ** Constructor **
    SquaresLookup(const Bitboards& bitboards);


    // ** Getters & Setters **
    inline PieceType getPieceTypeAtIndex(int index) const
    {
        return _squaresLookup[index];
    }

    inline void setPieceTypeAtIndex(int index, PieceType pieceType)
    {
        _squaresLookup[index] = pieceType;
    }

    // ** Functions **
    void fillSquaresLookupFromBitboards(const Bitboards& bitboards);
    
private:

    // ** Data **
    std::vector<PieceType> _squaresLookup;
};

} // namespace board