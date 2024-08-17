#include "QueenGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {

QueenGenerator::QueenGenerator(
    board::Bitboards& bitboards,
    RayLogic* rayLogic) 
    : _bitboards(bitboards)
    ,  _rayLogic(rayLogic) 
{
    _queenIndices.reserve(64);
    _straightRayBitmasks = masks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = masks::getAllDiagonalRayBitmasks();
}

void QueenGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    utils::getBitIndices(_queenIndices, isWhite ? _bitboards.getWhiteQueensBitboard()
                                                : _bitboards.getBlackQueensBitboard());

    for (int currentQueenIndex : _queenIndices) {
        masks::StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
        masks::DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
        
        int queenRank = utils::rankFromBitIndex(currentQueenIndex);
        int queenFile = utils::fileFromBitIndex(currentQueenIndex);

        _rayLogic->getMovesFromStraightRay(
            straightRays.north,
            true,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );

        _rayLogic->getMovesFromStraightRay(
            straightRays.east,
            false,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );

        _rayLogic->getMovesFromStraightRay(
            straightRays.south,
            false,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );

        _rayLogic->getMovesFromStraightRay(
            straightRays.west,
            true,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );

        _rayLogic->getMovesFromDiagonalRay(
            diagonalRays.northEast,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );

        _rayLogic->getMovesFromDiagonalRay(
            diagonalRays.southEast,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );

        _rayLogic->getMovesFromDiagonalRay(
            diagonalRays.southWest,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );
        
        _rayLogic->getMovesFromDiagonalRay(
            diagonalRays.northWest,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList
        );
    }
}

} // namespace movegen