#include "ChessEngine/src/logic/movegen/BishopGenerator.h"

#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {

BishopGenerator::BishopGenerator(const model::Bitboards& bitboards, const model::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _diagonalRayBitmasks = bitmasks::getAllDiagonalRayBitmasks();
}

void BishopGenerator::generate(
    bool isWhite,
    model::Movelist& moveListRef)
{
    std::vector<int>& bishopIndices = utils::Containers::getPiecePositionIndices();

    utils::getBitIndices(bishopIndices, isWhite ? _bitboardsRef.getWhiteBishopsBitboard()
                                                : _bitboardsRef.getBlackBishopsBitboard());

    for (int currentBishopIndex : bishopIndices) {
        bitmasks::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
        int bishopRank = utils::rankFromBitIndex(currentBishopIndex);
        int bishopFile = utils::fileFromBitIndex(currentBishopIndex);

        RayLogic::addMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
    }
}

} // namespace logic