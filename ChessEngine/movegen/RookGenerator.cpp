#include "ChessEngine/utils/Containers.h"

#include "ChessEngine/movegen/RookGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/utils/BitBasics.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {
RookGenerator::RookGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks)
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _straightRayBitmasks = masks::getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    Movelist& moveListRef)
{
    std::vector<int>& rookIndices = utils::Containers::getPiecePositionIndices();

    masks::StraightRays rays;

    utils::getBitIndices(rookIndices, isWhite ? _bitboardsRef.getWhiteRooksBitboard()
                                              : _bitboardsRef.getBlackRooksBitboard());

    // Loop through all rooks and isolate them
    for (int currentRookIndex : rookIndices) {
        rays = _straightRayBitmasks[currentRookIndex];
        int rookRank = utils::rankFromBitIndex(currentRookIndex);
        int rookFile = utils::fileFromBitIndex(currentRookIndex);

        RayLogic::addMovesFromStraightRay(
            rays.north,
            true, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.east,
            false, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.south,
            false, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.west,
            true, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );
    }
}

} // namespace movegen