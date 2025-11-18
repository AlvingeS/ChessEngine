#include "logic/movegen/check_detection.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/movegen/bitmasks/knight_bitmasks.h"
#include "logic/movegen/bitmasks/pawn_bitmasks.h"

namespace logic {

CheckDetection::CheckDetection(model::Board& board)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.state_bitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
    _knightBitmasks = KnightBitmasks::getAllKnightBitmasks();
    _whitePawnCaptureMoveBitmasks = PawnBitmasks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnCaptureMoveBitmasks = PawnBitmasks::getAllCapturePawnMoveBitmasks(false);
}

bool CheckDetection::isInCheck(bool isWhite) const
{
    int kingIndex, opponentKingIndex, kingRankDiff, kingFileDiff;

    kingIndex = BitBasics::indexOfLSB(isWhite ? _bitboards.get_w_king_bitboard()
                                   : _bitboards.get_b_king_bitboard());

    // Check if any opponent rooks or queens are attacking the king
    RayBitmasks::StraightRays straightRays = _straightRayBitmasks[kingIndex];
    bitmask opponentRooksAndQueens = isWhite ? _bitboards.get_b_rooks_bitboard() | _bitboards.get_b_queens_bitboard()
                                             : _bitboards.get_w_rooks_bitboard() | _bitboards.get_w_queens_bitboard();

    if (RayLogic::checkStraightRay(straightRays.north, true, opponentRooksAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.east, false, opponentRooksAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.south, false, opponentRooksAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.west, true, opponentRooksAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    // Check if any opponent bishops or queens are attacking the king
    RayBitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];
    bitmask opponentBishopsAndQueens = isWhite ? _bitboards.get_b_bishops_bitboard() | _bitboards.get_b_queens_bitboard() 
                                               : _bitboards.get_w_bishops_bitboard() | _bitboards.get_w_queens_bitboard();

    if (RayLogic::checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens, _stateBitmasks.get_occupied_pieces_bitmask()))
        return true;

    // Check if any opponent knights are attacking the king
    bitmask knightMoves = _knightBitmasks[kingIndex];
    bitmask opponentKnights = isWhite ? _bitboards.get_b_knights_bitboard() 
                                      : _bitboards.get_w_knights_bitboard();

    if ((knightMoves & opponentKnights) != 0)
        return true;
    
    // Check if any opponent pawns are attacking the king
    bitmask opponentPawns = isWhite ? _bitboards.get_b_pawns_bitboard() 
                                    : _bitboards.get_w_pawns_bitboard();
    
    bitmask pawnAttackingMoves = isWhite ? _whitePawnCaptureMoveBitmasks[kingIndex] 
                                         : _blackPawnCaptureMoveBitmasks[kingIndex];

    if ((pawnAttackingMoves & opponentPawns) != 0)
        return true;

    // Check if the king is in check from an adjacent king
    opponentKingIndex = BitBasics::indexOfLSB(isWhite ? _bitboards.get_b_king_bitboard()
                                           : _bitboards.get_w_king_bitboard());

    kingRankDiff = ChessUtils::abs(ChessUtils::rankFromBitIndex(kingIndex) - ChessUtils::rankFromBitIndex(opponentKingIndex));
    kingFileDiff = ChessUtils::abs(ChessUtils::fileFromBitIndex(kingIndex) - ChessUtils::fileFromBitIndex(opponentKingIndex));

    int manhattanDistance = kingRankDiff + kingFileDiff;

    if (manhattanDistance <= 1)
        return true;

    if (manhattanDistance == 2 && kingRankDiff == 1 && kingFileDiff == 1)
        return true;

    return false;
}

} // namespace logic