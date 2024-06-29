#include "SquaresLookup.h"

namespace game {
    SquaresLookup::SquaresLookup(BitBoards& bitboards) {
        fillSquaresLookup(bitboards);
    }

    void SquaresLookup::fillSquaresLookup(BitBoards& bitboards) {
        _squaresLookup = std::vector<PieceType>(64, PieceType::EMPTY);

        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 12; j++) {
                if (bitboards.getBitboardFromIndex(j) & (1ULL << i)) {
                    _squaresLookup[i] = intToPieceType(j);
                }
            }
        }
    }
}