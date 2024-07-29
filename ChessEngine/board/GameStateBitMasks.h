#pragma once

#include "ChessEngine/common.h"

#include "BitBoards.h"

namespace board {

class GameStateBitMasks {

public:
    // ** Constructor **
    GameStateBitMasks(BitBoards& bitboards);

    // ** Getters & Setters **
    inline U64& getWhitePiecesBitmask() 
    {
        return _whitePiecesBitmask;
    }

    inline U64& getBlackPiecesBitmask() 
    {
        return _blackPiecesBitmask;
    }

    inline U64& getOccupiedPiecesBitmask() 
    {
        return _occupiedPiecesBitmask;
    }

    inline U64& getEmptySquaresBitmask() 
    {
        return _emptySquaresBitmask;
    }
    
    void fillWhitePiecesBitmask(BitBoards& bitboards);
    void fillBlackPiecesBitmask(BitBoards& bitboards);
    
    void fillOccupiedPiecesBitmask() 
    {
        _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
    }

    void fillEmptySquaresBitmask() 
    {
        _emptySquaresBitmask = ~_occupiedPiecesBitmask;
    }

private:
    
    // ** Data **
    U64 _whitePiecesBitmask;
    U64 _blackPiecesBitmask;
    U64 _occupiedPiecesBitmask;
    U64 _emptySquaresBitmask;
};

} // namespace board