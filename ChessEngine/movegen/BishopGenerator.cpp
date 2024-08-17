#include "BishopGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {

BishopGenerator::BishopGenerator(const board::Bitboards& bitboards, RayLogic* rayLogic) 
    : _bitboardsRef(bitboards)
    , _rayLogic(rayLogic) 
{
    _bishopIndices.reserve(64);
    _diagonalRayBitmasks = masks::getAllDiagonalRayBitmasks();
}

void BishopGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    utils::getBitIndices(_bishopIndices, isWhite ? _bitboardsRef.getWhiteBishopsBitboard()
                                                 : _bitboardsRef.getBlackBishopsBitboard());

    for (int currentBishopIndex : _bishopIndices) {
        masks::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
        int bishopRank = utils::rankFromBitIndex(currentBishopIndex);
        int bishopFile = utils::fileFromBitIndex(currentBishopIndex);

        _rayLogic->getMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);
        _rayLogic->getMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);;
        _rayLogic->getMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);;
        _rayLogic->getMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);
    }
}

} // namespace movegen