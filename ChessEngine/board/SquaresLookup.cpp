#include "SquaresLookup.h"

namespace board {

SquaresLookup::SquaresLookup(const Bitboards& bitboards) 
{
    fillSquaresLookupFromBitboards(bitboards);
}

void SquaresLookup::fillSquaresLookupFromBitboards(const Bitboards& bitboards) 
{
    _squaresLookup = std::vector<PieceType>(64, PieceType::EMPTY);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 12; j++) {
            if (bitboards.getBitboardFromIndex(j) & (1ULL << i)) {
                _squaresLookup[i] = intToPieceType(j);
                break;
            }
        }
    }
}

} // namespace board