#include "ChessEngine/src/logic/movegen/CheckDetection.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

CheckDetection::CheckDetection(
    const model::Bitboards& bitboards,
    const model::GameStateBitmasks& gameStateBitmasks)
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _straightRayBitmasks = bitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = bitmasks::getAllDiagonalRayBitmasks();
    _knightBitmasks = bitmasks::getAllKnightBitmasks();
    _whitePawnCaptureMoveBitmasks = bitmasks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnCaptureMoveBitmasks = bitmasks::getAllCapturePawnMoveBitmasks(false);
}

bool CheckDetection::isInCheck(bool isWhite) const
{
    int kingIndex, opponentKingIndex, kingRankDiff, kingFileDiff;

    kingIndex = utils::indexOfLSB(isWhite ? _bitboardsRef.getWhiteKingBitboard()
                                          : _bitboardsRef.getBlackKingBitboard());

    opponentKingIndex = utils::indexOfLSB(isWhite ? _bitboardsRef.getBlackKingBitboard()
                                                  : _bitboardsRef.getWhiteKingBitboard());

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

    bitmasks::StraightRays straightRays = _straightRayBitmasks[kingIndex];
    bitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];

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

    if (RayLogic::checkStraightRay(straightRays.north, true, opponentRooksAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.east, false, opponentRooksAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.south, false, opponentRooksAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.west, true, opponentRooksAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens, _gameStateBitmasksRef.getOccupiedPiecesBitmask()))
        return true;

    return false;
}

} // namespace logic