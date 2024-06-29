#include "GameStateBitMasks.h"

namespace game {
    GameStateBitMasks::GameStateBitMasks(BitBoards& bitboards) {
        fillWhitePiecesBitmask(bitboards);
        fillBlackPiecesBitmask(bitboards);
        fillOccupiedPiecesBitmask();
        fillEmptySquaresBitmask();
    }

    void GameStateBitMasks::fillWhitePiecesBitmask(BitBoards& bitboards) {
        U64 w_all = 0;
        for (int i = 0; i < 6; i++) {
            w_all |= bitboards.getBitboardFromIndex(i);
        }

        _whitePiecesBitmask = w_all;
    }

    void GameStateBitMasks::fillBlackPiecesBitmask(BitBoards& bitboards) {
        U64 b_all = 0;
        for (int i = 6; i < 12; i++) {
            b_all |= bitboards.getBitboardFromIndex(i);
        }

        _blackPiecesBitmask = b_all;
    }
}