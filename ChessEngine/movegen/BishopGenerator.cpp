#include "ChessEngine/utils/Containers.h"

#include "BishopGenerator.h"

#include "ChessEngine/utils/BitBasics.h"
#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/movegen/RayLogic.h"

namespace movegen {

BishopGenerator::BishopGenerator(const board::Bitboards& bitboards, const board::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _diagonalRayBitmasks = masks::getAllDiagonalRayBitmasks();
}

void BishopGenerator::generate(
    bool isWhite,
    Movelist& moveListRef)
{
    std::vector<int>& bishopIndices = utils::Containers::getPiecePositionIndices();

    utils::getBitIndices(bishopIndices, isWhite ? _bitboardsRef.getWhiteBishopsBitboard()
                                                : _bitboardsRef.getBlackBishopsBitboard());

    for (int currentBishopIndex : bishopIndices) {
        masks::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
        int bishopRank = utils::rankFromBitIndex(currentBishopIndex);
        int bishopFile = utils::fileFromBitIndex(currentBishopIndex);

        RayLogic::addMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveListRef, _gameStateBitmasksRef.getWhitePiecesBitmask(), _gameStateBitmasksRef.getOccupiedPiecesBitmask());
    }
}

} // namespace movegen