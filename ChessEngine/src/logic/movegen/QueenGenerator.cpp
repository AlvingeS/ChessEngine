#include "ChessEngine/src/logic/movegen/QueenGenerator.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {

QueenGenerator::QueenGenerator(
    const model::Bitboards& bitboards,
    const model::StateBitmasks& stateBitmasks) 
    : _bitboardsRef(bitboards)
    , _stateBitmasksRef(stateBitmasks)
{
    _straightRayBitmasks = getAllStraightRayBitmasks();
    _diagonalRayBitmasks = getAllDiagonalRayBitmasks();
}

void QueenGenerator::generate(
    bool isWhite,
    model::Movelist& moveListRef)
{
    std::vector<int>& queenIndices = Containers::getPiecePositionIndices();

    getBitIndices(queenIndices, isWhite ? _bitboardsRef.getWhiteQueensBitboard()
                                               : _bitboardsRef.getBlackQueensBitboard());

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
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );

        addMovesFromStraightRay(
            straightRays.east,
            false,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );

        addMovesFromStraightRay(
            straightRays.south,
            false,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );

        addMovesFromStraightRay(
            straightRays.west,
            true,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );

        addMovesFromDiagonalRay(
            diagonalRays.northEast,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );

        addMovesFromDiagonalRay(
            diagonalRays.southEast,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );

        addMovesFromDiagonalRay(
            diagonalRays.southWest,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );
        
        addMovesFromDiagonalRay(
            diagonalRays.northWest,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _stateBitmasksRef.getWhitePiecesBitmask(),
            _stateBitmasksRef.getOccupiedPiecesBitmask()
        );
    }
}

} // namespace logic