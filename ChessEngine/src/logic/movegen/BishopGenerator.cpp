#include "ChessEngine/src/logic/movegen/BishopGenerator.h"

#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

#include "ChessEngine/src/model/board/PieceType.h"
#include "ChessEngine/src/model/board/Board.h"
#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/model/move/Movelist.h"

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

    getBitIndices(bishopIndices, isWhite ? _bitboards.getWhiteBishopsBitboard()
                                         : _bitboards.getBlackBishopsBitboard());

    for (int currentBishopIndex : bishopIndices) {
        RayBitmasks::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
        int bishopRank = rankFromBitIndex(currentBishopIndex);
        int bishopFile = fileFromBitIndex(currentBishopIndex);

        RayLogic::addMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
    }
}

} // namespace logic