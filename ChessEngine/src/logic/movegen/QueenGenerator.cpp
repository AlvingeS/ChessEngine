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
    _straightRayBitmasks = getAllStraightRayBitmasks();
    _diagonalRayBitmasks = getAllDiagonalRayBitmasks();
}

void QueenGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& queenIndices = Containers::getPiecePositionIndices();

    getBitIndices(queenIndices, isWhite ? _bitboards.getWhiteQueensBitboard()
                                               : _bitboards.getBlackQueensBitboard());

    for (int currentQueenIndex : queenIndices) {
        StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
        DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
        
        int queenRank = rankFromBitIndex(currentQueenIndex);
        int queenFile = fileFromBitIndex(currentQueenIndex);

        addMovesFromStraightRay(
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

        addMovesFromStraightRay(
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

        addMovesFromStraightRay(
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

        addMovesFromStraightRay(
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

        addMovesFromDiagonalRay(
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

        addMovesFromDiagonalRay(
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

        addMovesFromDiagonalRay(
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
        
        addMovesFromDiagonalRay(
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