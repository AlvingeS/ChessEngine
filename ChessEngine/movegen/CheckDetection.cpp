#include "CheckDetection.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {
CheckDetection::CheckDetection(
    board::BitBoards& bitboards,
    RayLogic* rayLogic)
    : _bitboards(bitboards)
    , _rayLogic(rayLogic) 
{
    _straightRayBitmasks = masks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = masks::getAllDiagonalRayBitmasks();
    _knightBitmasks = masks::getAllKnightBitMasks();
    _whitePawnCaptureMoveBitmasks = masks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnCaptureMoveBitmasks = masks::getAllCapturePawnMoveBitmasks(false);
}

bool CheckDetection::isInCheck(bool isWhite) 
{
    int kingIndex, opponentKingIndex, kingRankDiff, kingFileDiff;

    kingIndex = utils::indexOfLSB(isWhite ? _bitboards.getWhiteKingBitboard()
                                          : _bitboards.getBlackKingBitboard());

    opponentKingIndex = utils::indexOfLSB(isWhite ? _bitboards.getBlackKingBitboard()
                                                  : _bitboards.getWhiteKingBitboard());

    kingRankDiff = utils::rankFromBitIndex(kingIndex) - utils::rankFromBitIndex(opponentKingIndex);
    kingFileDiff = utils::fileFromBitIndex(kingIndex) - utils::fileFromBitIndex(opponentKingIndex);

    kingRankDiff = kingRankDiff < 0 ? -kingRankDiff 
                                    : kingRankDiff;

    kingFileDiff = kingFileDiff < 0 ? -kingFileDiff 
                                    : kingFileDiff;

    int manhattanDistance = kingRankDiff + kingFileDiff;

    if (manhattanDistance <= 2) {
        if (kingRankDiff == 0 || kingFileDiff == 0) {
            if (manhattanDistance == 1) {
                return true;
            }
        } else {
            return true;
        }
    }

    masks::StraightRays straightRays = _straightRayBitmasks[kingIndex];
    masks::DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];

    bitmask knightMoves = _knightBitmasks[kingIndex];

    bitmask pawnAttackingMoves = isWhite ? _whitePawnCaptureMoveBitmasks[kingIndex] 
                                     : _blackPawnCaptureMoveBitmasks[kingIndex];
    bitmask opponentRooksAndQueens = isWhite ? _bitboards.getBlackRooksBitboard() | _bitboards.getBlackQueensBitboard()
                                         : _bitboards.getWhiteRooksBitboard() | _bitboards.getWhiteQueensBitboard();

    bitmask opponentBishopsAndQueens = isWhite ? _bitboards.getBlackBishopsBitboard() | _bitboards.getBlackQueensBitboard() 
                                           : _bitboards.getWhiteBishopsBitboard() | _bitboards.getWhiteQueensBitboard();

    bitmask opponentPawns = isWhite ? _bitboards.getBlackPawnsBitboard() : _bitboards.getWhitePawnsBitboard();
    
    if ((pawnAttackingMoves & opponentPawns) != 0)
        return true;

    bitmask opponentKnights = isWhite ? _bitboards.getBlackKnightsBitboard() 
                                  : _bitboards.getWhiteKnightsBitboard();

    if ((knightMoves & opponentKnights) != 0)
        return true;

    if (_rayLogic->checkStraightRay(straightRays.north, true, opponentRooksAndQueens))
        return true;

    if (_rayLogic->checkStraightRay(straightRays.east, false, opponentRooksAndQueens))
        return true;

    if (_rayLogic->checkStraightRay(straightRays.south, false, opponentRooksAndQueens))
        return true;

    if (_rayLogic->checkStraightRay(straightRays.west, true, opponentRooksAndQueens))
        return true;

    if (_rayLogic->checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens))
        return true;

    if (_rayLogic->checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens))
        return true;

    if (_rayLogic->checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens))
        return true;

    if (_rayLogic->checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens))
        return true;

    return false;
}

} // namespace movegen