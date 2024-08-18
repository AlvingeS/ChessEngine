#pragma once

#include "ChessEngine/common.h"

#include "Bitboards.h"

namespace board {

class GameStateBitmasks {

public:
    GameStateBitmasks(const Bitboards& bitboards);
    
    inline bool whitePiecesBitIsSet(int square) const 
    {
        return (_whitePiecesBitmask & (1ULL << square)) != 0;
    }

    inline bool blackPiecesBitIsSet(int square) const 
    {
        return (_blackPiecesBitmask & (1ULL << square)) != 0;
    }

    inline void setWhitePiecesBit(int square) 
    {
        assert(!whitePiecesBitIsSet(square));
        _whitePiecesBitmask |= (1ULL << square);
    }

    inline void clearWhitePiecesBit(int square) 
    {
        assert(whitePiecesBitIsSet(square));
        _whitePiecesBitmask &= ~(1ULL << square);
    }

    inline const bitmask& getWhitePiecesBitmask() const 
    {
        return _whitePiecesBitmask;
    }

    inline void setBlackPiecesBit(int square) 
    {
        assert(!blackPiecesBitIsSet(square));
        _blackPiecesBitmask |= (1ULL << square);
    }

    inline void clearBlackPiecesBit(int square) 
    {
        assert(blackPiecesBitIsSet(square));
        _blackPiecesBitmask &= ~(1ULL << square);
    }

    inline const bitmask& getBlackPiecesBitmask() const 
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

    inline const bitmask& getOccupiedPiecesBitmask() const 
    {
        return _occupiedPiecesBitmask;
    }

    inline const bitmask& getEmptySquaresBitmask() const 
    {
        return _emptySquaresBitmask;
    }

    void fillWhitePiecesFromBitboards(const Bitboards& bitboards);
    void fillBlackPiecesFromBitboards(const Bitboards& bitboards);

private:

    bitmask _whitePiecesBitmask;
    bitmask _blackPiecesBitmask;
    bitmask _occupiedPiecesBitmask;
    bitmask _emptySquaresBitmask;
};

} // namespace board