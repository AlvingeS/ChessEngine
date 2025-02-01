#pragma once
#include "ChessEngine/src/common.h"

namespace model {

class Bitboards;

class StateBitmasks {

public:
    StateBitmasks(const Bitboards& bitboards);

    bool whitePiecesBitIsSet(int index) const;
    bool blackPiecesBitIsSet(int index) const;
    const bitmask& getWhitePiecesBitmask() const;
    const bitmask& getBlackPiecesBitmask() const;
    const bitmask& getOccupiedPiecesBitmask() const;
    const bitmask& getEmptySquaresBitmask() const;
    
    void setWhitePiecesBit(int index);
    void clearWhitePiecesBit(int index);
    void setBlackPiecesBit(int index);
    void clearBlackPiecesBit(int index);
    void updOccupiedAndEmptySquaresBitmasks();
    void fillWhitePiecesBitmaskFromBitboards(const Bitboards& bitboards);
    void fillBlackPiecesBitmaskFromBitboards(const Bitboards& bitboards);

private:
    void updOccupiedPiecesBitmask();
    void updEmptySquaresBitmask();

    bitmask _whitePiecesBitmask;
    bitmask _blackPiecesBitmask;
    bitmask _occupiedPiecesBitmask;
    bitmask _emptySquaresBitmask;
};

} // namespace model

#include "ChessEngine/src/model/board/StateBitmasks.inl"