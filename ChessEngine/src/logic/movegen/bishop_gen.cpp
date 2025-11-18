#include "logic/movegen/bishop_gen.h"

#include "logic/movegen/ray_logic.h"

#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/movegen/utils/chess_utils.h"

#include "model/position/piece_type.h"
#include "model/position/board.h"
#include "model/move/move.h"
#include "model/move/movelist.h"

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