#include "ChessEngine/src/logic/movegen/RookGenerator.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

RookGenerator::RookGenerator(
    const model::Bitboards& bitboards,
    const model::StateBitmasks& stateBitmasks)
    : _bitboards(bitboards)
    , _stateBitmasks(stateBitmasks)
{
    _straightRayBitmasks = getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& rookIndices = Containers::getPiecePositionIndices();

    StraightRays rays;

    getBitIndices(rookIndices, isWhite ? _bitboards.getWhiteRooksBitboard()
                                              : _bitboards.getBlackRooksBitboard());

    // Loop through all rooks and isolate them
    for (int currentRookIndex : rookIndices) {
        rays = _straightRayBitmasks[currentRookIndex];
        int rookRank = rankFromBitIndex(currentRookIndex);
        int rookFile = fileFromBitIndex(currentRookIndex);

        addMovesFromStraightRay(
            rays.north,
            true, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        addMovesFromStraightRay(
            rays.east,
            false, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        addMovesFromStraightRay(
            rays.south,
            false, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        addMovesFromStraightRay(
            rays.west,
            true, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );
    }
}

} // namespace logic