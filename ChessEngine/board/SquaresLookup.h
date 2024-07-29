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
    inline PieceType getPieceTypeAtIndex(const int index) 
    {
        return _squaresLookup[index];
    }

    inline void setPieceTypeAtIndex(const int index, const PieceType pieceType) 
    {
        _squaresLookup[index] = pieceType;
    }

    // TODO: Should this really by a thing?
    inline std::vector<PieceType>& getSquaresLookup() 
    {
        return _squaresLookup;
    }

    // ** Functions **
    void fillSquaresLookup(const BitBoards& bitboards);
    
private:

    // ** Data **
    std::vector<PieceType> _squaresLookup;
};

} // namespace board