#include "GameStateBitMasks.h"

namespace board {

GameStateBitMasks::GameStateBitMasks(const BitBoards& bitboards) 
{
    fillWhitePiecesFromBitboards(bitboards);
    fillBlackPiecesFromBitboards(bitboards);
    updOccupiedAndEmptySquaresBitmasks();
}

void GameStateBitMasks::fillWhitePiecesFromBitboards(const BitBoards& bitboards) 
{
    U64 w_all = 0;
    for (int i = 0; i < 6; i++) {
        w_all |= bitboards.getBitboardFromIndex(i);
    }

    _whitePiecesBitmask = w_all;
}

void GameStateBitMasks::fillBlackPiecesFromBitboards(const BitBoards& bitboards) 
{
    U64 b_all = 0;
    for (int i = 6; i < 12; i++) {
        b_all |= bitboards.getBitboardFromIndex(i);
    }

    _blackPiecesBitmask = b_all;
}

} // namespace board