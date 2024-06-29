#include "BishopGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    BishopGenerator::BishopGenerator(game::ChessBoard& board, RayLogic* rayLogic) 
        : _board(board),
          _rayLogic(rayLogic) {
        _bishopIndices.reserve(64);
        _diagonalRayBitmasks = bits::getAllDiagonalRayBitmasks();
    }

    void BishopGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        bits::getBitIndices(_bishopIndices, isWhite ? _board.getWhiteBishopsBitboard()
                                                    : _board.getBlackBishopsBitboard());

        for (int currentBishopIndex : _bishopIndices) {
            bits::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
            int bishopRank = bits::rankFromBitIndex(currentBishopIndex);
            int bishopFile = bits::fileFromBitIndex(currentBishopIndex);

            _rayLogic->getMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);
            _rayLogic->getMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);;
            _rayLogic->getMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);;
            _rayLogic->getMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList);
        }
    }
}