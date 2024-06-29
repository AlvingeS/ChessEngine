#include "RookGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    RookGenerator::RookGenerator(game::ChessBoard& board, RayLogic* rayLogic) 
        : _board(board),
          _rayLogic(rayLogic) {
        _rookIndices.reserve(64);
        _straightRayBitmasks = bits::getAllStraightRayBitmasks();
    }

    void RookGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        bits::StraightRays rays;

        bits::getBitIndices(_rookIndices, isWhite ? _board.getWhiteRooksBitboard()
                                                  : _board.getBlackRooksBitboard());

        // Loop through all rooks and isolate them
        for (int currentRookIndex : _rookIndices) {
            rays = _straightRayBitmasks[currentRookIndex];
            int rookRank = bits::rankFromBitIndex(currentRookIndex);
            int rookFile = bits::fileFromBitIndex(currentRookIndex);

            _rayLogic->getMovesFromStraightRay(rays.north, true, false, isWhite, currentRookIndex, rookRank, rookFile, moveList);
            _rayLogic->getMovesFromStraightRay(rays.east, false, true, isWhite, currentRookIndex, rookRank, rookFile, moveList);
            _rayLogic->getMovesFromStraightRay(rays.south, false, false, isWhite, currentRookIndex, rookRank, rookFile, moveList);;
            _rayLogic->getMovesFromStraightRay(rays.west, true, true, isWhite, currentRookIndex, rookRank, rookFile, moveList);
        }
    }
}