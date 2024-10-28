#include "ChessEngine/src/logic/movegen/RookGenerator.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

RookGenerator::RookGenerator(
    const model::Bitboards& bitboards,
    const model::GameStateBitmasks& gameStateBitmasks)
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _straightRayBitmasks = bitmasks::getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    model::Movelist& moveListRef)
{
    std::vector<int>& rookIndices = utils::Containers::getPiecePositionIndices();

    bitmasks::StraightRays rays;

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

} // namespace logic