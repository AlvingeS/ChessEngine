#pragma once

#include "ChessEngine/common.h"

#include "BitBoards.h"

namespace board {

class GameStateBitMasks {

public:
    // ** Constructor **
    GameStateBitMasks(const BitBoards& bitboards);
    
    inline bool whitePiecesBitIsSet(const int square) const 
    {
        return (_whitePiecesBitmask & (1ULL << square)) != 0;
    }

    inline bool blackPiecesBitIsSet(const int square) const 
    {
        return (_blackPiecesBitmask & (1ULL << square)) != 0;
    }

    inline void setWhitePiecesBit(const int square) 
    {
        assert(!whitePiecesBitIsSet(square));
        _whitePiecesBitmask |= (1ULL << square);
    }

    inline void clearWhitePiecesBit(const int square) 
    {
        assert(whitePiecesBitIsSet(square));
        _whitePiecesBitmask &= ~(1ULL << square);
    }

    inline const U64& getWhitePiecesBitmask() const 
    {
        return _whitePiecesBitmask;
    }

    inline void setBlackPiecesBit(const int square) 
    {
        assert(!blackPiecesBitIsSet(square));
        _blackPiecesBitmask |= (1ULL << square);
    }

    inline void clearBlackPiecesBit(const int square) 
    {
        assert(blackPiecesBitIsSet(square));
        _blackPiecesBitmask &= ~(1ULL << square);
    }

    inline const U64& getBlackPiecesBitmask() const 
    {
        return _blackPiecesBitmask;
    }

    inline void updOccupiedAndEmptySquaresBitmasks() 
    {
        updOccupiedPiecesBitmask();
        updEmptySquaresBitmask();
    }

    inline void updOccupiedPiecesBitmask() 
    {
        _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
    }

    inline void updEmptySquaresBitmask() 
    {
        _emptySquaresBitmask = ~_occupiedPiecesBitmask;
    }

    inline const U64& getOccupiedPiecesBitmask() const 
    {
        return _occupiedPiecesBitmask;
    }

    inline const U64& getEmptySquaresBitmask() const 
    {
        return _emptySquaresBitmask;
    }

    void fillWhitePiecesFromBitboards(const BitBoards& bitboards);
    void fillBlackPiecesFromBitboards(const BitBoards& bitboards);

private:
    
    // ** Data **
    U64 _whitePiecesBitmask;
    U64 _blackPiecesBitmask;
    U64 _occupiedPiecesBitmask;
    U64 _emptySquaresBitmask;
};

} // namespace board