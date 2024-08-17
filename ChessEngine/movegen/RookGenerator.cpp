#include "RookGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {
RookGenerator::RookGenerator(
    board::Bitboards& bitboards,
    RayLogic* rayLogic)
    : _bitboards(bitboards)
    , _rayLogic(rayLogic)
{
    _rookIndices.reserve(64);
    _straightRayBitmasks = masks::getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    masks::StraightRays rays;

    utils::getBitIndices(_rookIndices, isWhite 
                                       ? _bitboards.getWhiteRooksBitboard()
                                       : _bitboards.getBlackRooksBitboard());

    // Loop through all rooks and isolate them
    for (int currentRookIndex : _rookIndices) {
        rays = _straightRayBitmasks[currentRookIndex];
        int rookRank = utils::rankFromBitIndex(currentRookIndex);
        int rookFile = utils::fileFromBitIndex(currentRookIndex);

        _rayLogic->getMovesFromStraightRay(
            rays.north,
            true, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList
        );

        _rayLogic->getMovesFromStraightRay(
            rays.east,
            false, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList
        );

        _rayLogic->getMovesFromStraightRay(
            rays.south,
            false, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList
        );
        ;
        _rayLogic->getMovesFromStraightRay(
            rays.west,
            true, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList
        );
    }
}

} // namespace movegen