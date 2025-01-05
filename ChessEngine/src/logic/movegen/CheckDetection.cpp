#include "ChessEngine/src/logic/movegen/CheckDetection.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

CheckDetection::CheckDetection(
    const model::Bitboards& bitboards,
    const model::StateBitmasks& stateBitmasks)
    : _bitboards(bitboards)
    , _stateBitmasks(stateBitmasks)
{
    _straightRayBitmasks = getAllStraightRayBitmasks();
    _diagonalRayBitmasks = getAllDiagonalRayBitmasks();
    _knightBitmasks = getAllKnightBitmasks();
    _whitePawnCaptureMoveBitmasks = getAllCapturePawnMoveBitmasks(true);
    _blackPawnCaptureMoveBitmasks = getAllCapturePawnMoveBitmasks(false);
}

bool CheckDetection::isInCheck(bool isWhite) const
{
    int kingIndex, opponentKingIndex, kingRankDiff, kingFileDiff;

    kingIndex = indexOfLSB(isWhite ? _bitboards.getWhiteKingBitboard()
                                          : _bitboards.getBlackKingBitboard());

    opponentKingIndex = indexOfLSB(isWhite ? _bitboards.getBlackKingBitboard()
                                                  : _bitboards.getWhiteKingBitboard());

    kingRankDiff = rankFromBitIndex(kingIndex) - rankFromBitIndex(opponentKingIndex);
    kingFileDiff = fileFromBitIndex(kingIndex) - fileFromBitIndex(opponentKingIndex);

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

    StraightRays straightRays = _straightRayBitmasks[kingIndex];
    DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];

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

    if (checkStraightRay(straightRays.north, true, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.east, false, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.south, false, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.west, true, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    return false;
}

} // namespace logic