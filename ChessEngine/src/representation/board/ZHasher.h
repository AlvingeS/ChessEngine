#pragma once

#include "ChessEngine/src/representation/board/SquaresLookup.h"
#include "ChessEngine/src/representation/board/PieceType.h"

namespace representation {
namespace board {

class ZHasher {

public:
    ZHasher(SquaresLookup& squaresLookup);

private:
    void computeInitialHash();

    uint64_t _randBoardPieceTypeNums[64][12];
    
    // In order:
    //      0 - White King side
    //      1 - White Queen side
    //      2 - Black King side	
    //      3 - Black Queen side
    uint64_t _randCastleRightsNums[4];
    
    uint64_t _randEnPassantFileNums[8];
    uint64_t _randIsWhiteNum;
    uint64_t _hash;

    SquaresLookup& _squaresLookupRef;
};

} // namespace board
} // namespace representation
