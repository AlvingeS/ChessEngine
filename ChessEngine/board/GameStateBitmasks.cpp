#include "GameStateBitmasks.h"

namespace board {

GameStateBitmasks::GameStateBitmasks(const Bitboards& bitboards) 
{
    fillWhitePiecesFromBitboards(bitboards);
    fillBlackPiecesFromBitboards(bitboards);
    updOccupiedAndEmptySquaresBitmasks();
}

void GameStateBitmasks::fillWhitePiecesFromBitboards(const Bitboards& bitboards) 
{
    bitmask w_all = 0;
    for (int i = 0; i < 6; i++) {
        w_all |= bitboards.getBitboardFromIndex(i);
    }

    _whitePiecesBitmask = w_all;
}

void GameStateBitmasks::fillBlackPiecesFromBitboards(const Bitboards& bitboards) 
{
    bitmask b_all = 0;
    for (int i = 6; i < 12; i++) {
        b_all |= bitboards.getBitboardFromIndex(i);
    }

    _blackPiecesBitmask = b_all;
}

} // namespace board