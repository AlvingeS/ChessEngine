#include "ChessEngine/src/logic/movegen/CheckDetection.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

CheckDetection::CheckDetection(
    const model::Bitboards& bitboards,
    const model::StateBitmasks& stateBitmasks)
    : _bitboardsRef(bitboards)
    , _stateBitmasksRef(stateBitmasks)
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

    kingIndex = indexOfLSB(isWhite ? _bitboardsRef.getWhiteKingBitboard()
                                          : _bitboardsRef.getBlackKingBitboard());

    opponentKingIndex = indexOfLSB(isWhite ? _bitboardsRef.getBlackKingBitboard()
                                                  : _bitboardsRef.getWhiteKingBitboard());

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
    bitmask opponentRooksAndQueens = isWhite ? _bitboardsRef.getBlackRooksBitboard() | _bitboardsRef.getBlackQueensBitboard()
                                         : _bitboardsRef.getWhiteRooksBitboard() | _bitboardsRef.getWhiteQueensBitboard();

    bitmask opponentBishopsAndQueens = isWhite ? _bitboardsRef.getBlackBishopsBitboard() | _bitboardsRef.getBlackQueensBitboard() 
                                           : _bitboardsRef.getWhiteBishopsBitboard() | _bitboardsRef.getWhiteQueensBitboard();

    bitmask opponentPawns = isWhite ? _bitboardsRef.getBlackPawnsBitboard() : _bitboardsRef.getWhitePawnsBitboard();

    if ((pawnAttackingMoves & opponentPawns) != 0)
        return true;

    bitmask opponentKnights = isWhite ? _bitboardsRef.getBlackKnightsBitboard() 
                                  : _bitboardsRef.getWhiteKnightsBitboard();

    if ((knightMoves & opponentKnights) != 0)
        return true;

    if (checkStraightRay(straightRays.north, true, opponentRooksAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.east, false, opponentRooksAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.south, false, opponentRooksAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.west, true, opponentRooksAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens, _stateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    return false;
}

} // namespace logic