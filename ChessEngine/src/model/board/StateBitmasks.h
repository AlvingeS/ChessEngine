#pragma once
#include "ChessEngine/src/common.h"

namespace model {

class Bitboards;

class StateBitmasks {

public:
    StateBitmasks(const Bitboards& bitboards);

    bool whitePiecesBitIsSet(int index) const;
    bool blackPiecesBitIsSet(int index) const;
    void setWhitePiecesBit(int index);
    void clearWhitePiecesBit(int index);
    const bitmask& getWhitePiecesBitmask() const;
    void setBlackPiecesBit(int index);
    void clearBlackPiecesBit(int index);
    const bitmask& getBlackPiecesBitmask() const;
    void updOccupiedAndEmptySquaresBitmasks();
    const bitmask& getOccupiedPiecesBitmask() const;
    const bitmask& getEmptySquaresBitmask() const;
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