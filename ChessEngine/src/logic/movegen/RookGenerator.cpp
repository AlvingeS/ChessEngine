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
    _straightRayBitmasks = getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    model::Movelist& moveListRef)
{
    std::vector<int>& rookIndices = Containers::getPiecePositionIndices();

    StraightRays rays;

    getBitIndices(rookIndices, isWhite ? _bitboardsRef.getWhiteRooksBitboard()
                                              : _bitboardsRef.getBlackRooksBitboard());

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
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        addMovesFromStraightRay(
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

        addMovesFromStraightRay(
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

        addMovesFromStraightRay(
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