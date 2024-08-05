#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"
#include "BitBoards.h"

namespace board {

class SquaresLookup {

public:
    // ** Constructor **
    SquaresLookup(const BitBoards& bitboards);


    // ** Getters & Setters **
    inline PieceType getPieceTypeAtIndex(const int index) const
    {
        return _squaresLookup[index];
    }

    inline void setPieceTypeAtIndex(const int index, const PieceType pieceType)
    {
        _squaresLookup[index] = pieceType;
    }

    // ** Functions **
    void fillSquaresLookupFromBitboards(const BitBoards& bitboards);
    
private:

    // ** Data **
    std::vector<PieceType> _squaresLookup;
};

} // namespace board