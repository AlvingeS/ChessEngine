#include "logic/movegen/RookGenerator.h"

#include "model/position/Board.h"

#include "logic/movegen/RayLogic.h"
#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/ChessUtils.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

RookGenerator::RookGenerator(model::Board& board)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& rookIndices = Containers::getPiecePositionIndices();

    RayBitmasks::StraightRays rays;

    BitBasics::getBitIndices(rookIndices, isWhite ? _bitboards.getWhiteRooksBitboard()
                                       : _bitboards.getBlackRooksBitboard());

    // Loop through all rooks and isolate them
    for (int currentRookIndex : rookIndices) {
        rays = _straightRayBitmasks[currentRookIndex];
        int rookRank = ChessUtils::rankFromBitIndex(currentRookIndex);
        int rookFile = ChessUtils::fileFromBitIndex(currentRookIndex);

        RayLogic::addMovesFromStraightRay(
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

        RayLogic::addMovesFromStraightRay(
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

        RayLogic::addMovesFromStraightRay(
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

        RayLogic::addMovesFromStraightRay(
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