#pragma once
#include "ChessEngine/src/common.h"

namespace model {

class Bitboards;

class StateBitmasks {

public:
    StateBitmasks(const Bitboards& bitboards);

    inline bool whitePiecesBitIsSet(int index) const 
    {
        return (_whitePiecesBitmask & (1ULL << index)) != 0;
    }

    inline bool blackPiecesBitIsSet(int index) const 
    {
        return (_blackPiecesBitmask & (1ULL << index)) != 0;
    }

    inline void setWhitePiecesBit(int index) 
    {
        _whitePiecesBitmask |= (1ULL << index);
    }

    inline void clearWhitePiecesBit(int index) 
    {
        _whitePiecesBitmask &= ~(1ULL << index);
    }

    inline const bitmask& getWhitePiecesBitmask() const 
    {
        return _whitePiecesBitmask;
    }

    inline void setBlackPiecesBit(int index) 
    {
        _blackPiecesBitmask |= (1ULL << index);
    }

    inline void clearBlackPiecesBit(int index) 
    {
        _blackPiecesBitmask &= ~(1ULL << index);
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

    inline const bitmask& getOccupiedPiecesBitmask() const 
    {
        return _occupiedPiecesBitmask;
    }

    inline const bitmask& getEmptySquaresBitmask() const 
    {
        return _emptySquaresBitmask;
    }

    void fillWhitePiecesBitmaskFromBitboards(const Bitboards& bitboards);

    void fillBlackPiecesBitmaskFromBitboards(const Bitboards& bitboards);

private:
    inline void updOccupiedPiecesBitmask() 
    {
        _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
    }

    inline void updEmptySquaresBitmask() 
    {
        _emptySquaresBitmask = ~_occupiedPiecesBitmask;
    }

    bitmask _whitePiecesBitmask;
    bitmask _blackPiecesBitmask;
    bitmask _occupiedPiecesBitmask;
    bitmask _emptySquaresBitmask;

};

} // namespace model