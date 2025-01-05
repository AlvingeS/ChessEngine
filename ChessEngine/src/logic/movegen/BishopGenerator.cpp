#include "ChessEngine/src/logic/movegen/BishopGenerator.h"

#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {

BishopGenerator::BishopGenerator(const model::Bitboards& bitboards, const model::StateBitmasks& stateBitmasks) 
    : _bitboards(bitboards)
    , _stateBitmasks(stateBitmasks)
{
    _diagonalRayBitmasks = getAllDiagonalRayBitmasks();
}

void BishopGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& bishopIndices = Containers::getPiecePositionIndices();

    getBitIndices(bishopIndices, isWhite ? _bitboards.getWhiteBishopsBitboard()
                                                : _bitboards.getBlackBishopsBitboard());

    for (int currentBishopIndex : bishopIndices) {
        DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
        int bishopRank = rankFromBitIndex(currentBishopIndex);
        int bishopFile = fileFromBitIndex(currentBishopIndex);

        addMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        addMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        addMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        addMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
    }
}

} // namespace logic