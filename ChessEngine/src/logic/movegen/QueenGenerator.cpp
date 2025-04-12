#include "ChessEngine/src/logic/movegen/QueenGenerator.h"

#include "ChessEngine/src/model/board/Board.h"

#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {

QueenGenerator::QueenGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
}

void QueenGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& queenIndices = Containers::getPiecePositionIndices();

    BitBasics::getBitIndices(queenIndices, isWhite ? _bitboards.getWhiteQueensBitboard()
                                               : _bitboards.getBlackQueensBitboard());

    for (int currentQueenIndex : queenIndices) {
        RayBitmasks::StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
        RayBitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
        
        int queenRank = rankFromBitIndex(currentQueenIndex);
        int queenFile = fileFromBitIndex(currentQueenIndex);

        RayLogic::addMovesFromStraightRay(
            straightRays.north,
            true,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.east,
            false,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.south,
            false,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.west,
            true,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northEast,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southEast,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southWest,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );
        
        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northWest,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );
    }
}

} // namespace logic