#include "ChessEngine/src/logic/movegen/QueenGenerator.h"

#include "ChessEngine/src/representation/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {
namespace movegen {

QueenGenerator::QueenGenerator(
    const representation::board::Bitboards& bitboards,
    const representation::board::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _straightRayBitmasks = bitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = bitmasks::getAllDiagonalRayBitmasks();
}

void QueenGenerator::generate(
    bool isWhite,
    representation::move::Movelist& moveListRef)
{
    std::vector<int>& queenIndices = utils::Containers::getPiecePositionIndices();

    utils::getBitIndices(queenIndices, isWhite ? _bitboardsRef.getWhiteQueensBitboard()
                                               : _bitboardsRef.getBlackQueensBitboard());

    for (int currentQueenIndex : queenIndices) {
        bitmasks::StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
        bitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
        
        int queenRank = utils::rankFromBitIndex(currentQueenIndex);
        int queenFile = utils::fileFromBitIndex(currentQueenIndex);

        RayLogic::addMovesFromStraightRay(
            straightRays.north,
            true,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.east,
            false,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.south,
            false,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.west,
            true,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northEast,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southEast,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southWest,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );
        
        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northWest,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveListRef,
            _gameStateBitmasksRef.getWhitePiecesBitmask(),
            _gameStateBitmasksRef.getOccupiedPiecesBitmask()
        );
    }
}

} // namespace movegen
} // namespace logic