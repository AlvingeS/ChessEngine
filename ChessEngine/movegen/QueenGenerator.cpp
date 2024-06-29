#include "QueenGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    QueenGenerator::QueenGenerator(game::ChessBoard& board, RayLogic* rayLogic) 
        : _board(board),
          _rayLogic(rayLogic) {
        _queenIndices.reserve(64);
        _straightRayBitmasks = bits::getAllStraightRayBitmasks();
        _diagonalRayBitmasks = bits::getAllDiagonalRayBitmasks();
    }

    void QueenGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        bits::getBitIndices(_queenIndices, isWhite ? _board.getWhiteQueensBitboard()
                                                   : _board.getBlackQueensBitboard());

        for (int currentQueenIndex : _queenIndices) {
            bits::StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
            bits::DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
            int queenRank = bits::rankFromBitIndex(currentQueenIndex);
            int queenFile = bits::fileFromBitIndex(currentQueenIndex);

            _rayLogic->getMovesFromStraightRay(straightRays.north, true, false, isWhite, currentQueenIndex, queenRank, queenFile, moveList);
            _rayLogic->getMovesFromStraightRay(straightRays.east, false, true, isWhite, currentQueenIndex, queenRank, queenFile, moveList);
            _rayLogic->getMovesFromStraightRay(straightRays.south, false, false, isWhite, currentQueenIndex, queenRank, queenFile, moveList);
            _rayLogic->getMovesFromStraightRay(straightRays.west, true, true, isWhite, currentQueenIndex, queenRank, queenFile, moveList);

            _rayLogic->getMovesFromDiagonalRay(diagonalRays.northEast, true, isWhite, currentQueenIndex, queenRank, queenFile, moveList);
            _rayLogic->getMovesFromDiagonalRay(diagonalRays.southEast, false, isWhite, currentQueenIndex, queenRank, queenFile, moveList);
            _rayLogic->getMovesFromDiagonalRay(diagonalRays.southWest, false, isWhite, currentQueenIndex, queenRank, queenFile, moveList);
            _rayLogic->getMovesFromDiagonalRay(diagonalRays.northWest, true, isWhite, currentQueenIndex, queenRank, queenFile, moveList);
        }
    }
}