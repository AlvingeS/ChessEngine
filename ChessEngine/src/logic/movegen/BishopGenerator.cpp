#include "logic/movegen/BishopGenerator.h"

#include "logic/movegen/RayLogic.h"

#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/BitBasics.h"
#include "logic/movegen/utils/ChessUtils.h"

#include "model/position/PieceType.h"
#include "model/position/Board.h"
#include "model/move/Move.h"
#include "model/move/Movelist.h"

namespace logic {

BishopGenerator::BishopGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
}

void BishopGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& bishopIndices = Containers::getPiecePositionIndices();

    BitBasics::getBitIndices(bishopIndices, isWhite ? _bitboards.getWhiteBishopsBitboard()
                                         : _bitboards.getBlackBishopsBitboard());

    for (int currentBishopIndex : bishopIndices) {
        RayBitmasks::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
        int bishopRank = ChessUtils::rankFromBitIndex(currentBishopIndex);
        int bishopFile = ChessUtils::fileFromBitIndex(currentBishopIndex);

        RayLogic::addMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
    }
}

} // namespace logic