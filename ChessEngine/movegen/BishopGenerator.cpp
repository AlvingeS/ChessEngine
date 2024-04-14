#include "BishopGenerator.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    BishopGenerator::BishopGenerator(game::ChessBoard& board, RayLogic* rayLogic) 
        : _board(board),
          _rayLogic(rayLogic) {
        _bishopIndices.reserve(64);
        _diagonalRayBitmasks = bits::getAllDiagonalRayBitmasks();
    }

    void BishopGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        game::PieceType currentPieceType = isWhite ? game::PieceType::W_BISHOP : game::PieceType::B_BISHOP;
        bits::getBitIndices(_bishopIndices, _board.getBitboard(currentPieceType));

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