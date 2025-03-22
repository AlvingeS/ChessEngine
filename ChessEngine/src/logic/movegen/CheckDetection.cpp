#include "ChessEngine/src/logic/movegen/CheckDetection.h"

#include "ChessEngine/src/model/board/Board.h"

#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KnightBitmasks.h"
#include "ChessEngine/src/logic/movegen/bitmasks/PawnBitmasks.h"

namespace logic {

CheckDetection::CheckDetection(model::Board& board)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
    _knightBitmasks = getAllKnightBitmasks();
    _whitePawnCaptureMoveBitmasks = getAllCapturePawnMoveBitmasks(true);
    _blackPawnCaptureMoveBitmasks = getAllCapturePawnMoveBitmasks(false);
}

bool CheckDetection::isInCheck(bool isWhite) const
{
    int kingIndex, opponentKingIndex, kingRankDiff, kingFileDiff;

    kingIndex = indexOfLSB(isWhite ? _bitboards.getWhiteKingBitboard()
                                   : _bitboards.getBlackKingBitboard());

    // Check if any opponent rooks or queens are attacking the king
    RayBitmasks::StraightRays straightRays = _straightRayBitmasks[kingIndex];
    bitmask opponentRooksAndQueens = isWhite ? _bitboards.getBlackRooksBitboard() | _bitboards.getBlackQueensBitboard()
                                             : _bitboards.getWhiteRooksBitboard() | _bitboards.getWhiteQueensBitboard();

    if (checkStraightRay(straightRays.north, true, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.east, false, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.south, false, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkStraightRay(straightRays.west, true, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    // Check if any opponent bishops or queens are attacking the king
    RayBitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];
    bitmask opponentBishopsAndQueens = isWhite ? _bitboards.getBlackBishopsBitboard() | _bitboards.getBlackQueensBitboard() 
                                               : _bitboards.getWhiteBishopsBitboard() | _bitboards.getWhiteQueensBitboard();

    if (checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    // Check if any opponent knights are attacking the king
    bitmask knightMoves = _knightBitmasks[kingIndex];
    bitmask opponentKnights = isWhite ? _bitboards.getBlackKnightsBitboard() 
                                      : _bitboards.getWhiteKnightsBitboard();

    if ((knightMoves & opponentKnights) != 0)
        return true;
    
    // Check if any opponent pawns are attacking the king
    bitmask opponentPawns = isWhite ? _bitboards.getBlackPawnsBitboard() 
                                    : _bitboards.getWhitePawnsBitboard();
    
    bitmask pawnAttackingMoves = isWhite ? _whitePawnCaptureMoveBitmasks[kingIndex] 
                                         : _blackPawnCaptureMoveBitmasks[kingIndex];

    if ((pawnAttackingMoves & opponentPawns) != 0)
        return true;

    // Check if the king is in check from an adjacent king
    opponentKingIndex = indexOfLSB(isWhite ? _bitboards.getBlackKingBitboard()
                                           : _bitboards.getWhiteKingBitboard());

    kingRankDiff = abs(rankFromBitIndex(kingIndex) - rankFromBitIndex(opponentKingIndex));
    kingFileDiff = abs(fileFromBitIndex(kingIndex) - fileFromBitIndex(opponentKingIndex));

    int manhattanDistance = kingRankDiff + kingFileDiff;

    if (manhattanDistance <= 1)
        return true;

    if (manhattanDistance == 2 && kingRankDiff == 1 && kingFileDiff == 1)
        return true;

    return false;
}

} // namespace logic