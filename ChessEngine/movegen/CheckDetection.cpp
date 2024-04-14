#include "CheckDetection.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    CheckDetection::CheckDetection(game::ChessBoard& board, RayLogic* rayLogic) 
        : _board(board),
          _rayLogic(rayLogic) {
        _straightRayBitmasks = bits::getAllStraightRayBitmasks();
        _diagonalRayBitmasks = bits::getAllDiagonalRayBitmasks();
        _knightBitmasks = bits::getAllKnightBitMasks();
        _whitePawnCaptureMoveBitmasks = bits::getAllCapturePawnMoveBitmasks(true);
        _blackPawnCaptureMoveBitmasks = bits::getAllCapturePawnMoveBitmasks(false);
    }

    bool CheckDetection::isInCheck(bool isWhite) {
        int kingIndex = bits::indexOfLSB(isWhite ? _board.getWhiteKingBitboard()
                                                 : _board.getBlackKingBitboard());

        int opponentKingIndex = bits::indexOfLSB(isWhite ? _board.getBlackKingBitboard()
                                                         : _board.getWhiteKingBitboard());

        int kingRankDiff = bits::rankFromBitIndex(kingIndex) - bits::rankFromBitIndex(opponentKingIndex);
        int kingFileDiff = bits::fileFromBitIndex(kingIndex) - bits::fileFromBitIndex(opponentKingIndex);

        kingRankDiff = kingRankDiff < 0 ? -kingRankDiff : kingRankDiff;
        kingFileDiff = kingFileDiff < 0 ? -kingFileDiff : kingFileDiff;
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

        bits::StraightRays straightRays = _straightRayBitmasks[kingIndex];
        bits::DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];
        bits::U64 knightMoves = _knightBitmasks[kingIndex];
        bits::U64 pawnAttackingMoves = isWhite ? _whitePawnCaptureMoveBitmasks[kingIndex] 
                                               : _blackPawnCaptureMoveBitmasks[kingIndex];
        bits::U64 opponentRooksAndQueens = isWhite ? _board.getBlackRooksBitboard() | _board.getBlackQueensBitboard()
                                                   : _board.getWhiteRooksBitboard() | _board.getWhiteQueensBitboard();

        bits::U64 opponentBishopsAndQueens = isWhite ? _board.getBlackBishopsBitboard() | _board.getBlackQueensBitboard() 
                                                     : _board.getWhiteBishopsBitboard() | _board.getWhiteQueensBitboard();

        bits::U64 opponentPawns = isWhite ? _board.getBlackPawnsBitboard() : _board.getWhitePawnsBitboard();
        if ((pawnAttackingMoves & opponentPawns) != 0) {
            return true;
        }

        bits::U64 opponentKnights = isWhite ? _board.getBlackKnightsBitboard() : _board.getWhiteKnightsBitboard();
        if ((knightMoves & opponentKnights) != 0) {
            return true;
        }

        if (_rayLogic->checkStraightRay(straightRays.north, true, opponentRooksAndQueens)) {
            return true;
        }

        if (_rayLogic->checkStraightRay(straightRays.east, false, opponentRooksAndQueens)) {
            return true;
        }

        if (_rayLogic->checkStraightRay(straightRays.south, false, opponentRooksAndQueens)) {
            return true;
        }

        if (_rayLogic->checkStraightRay(straightRays.west, true, opponentRooksAndQueens)) {
            return true;
        }

        if (_rayLogic->checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens)) {
            return true;
        }

        if (_rayLogic->checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens)) {
            return true;
        }

        if (_rayLogic->checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens)) {
            return true;
        }

        if (_rayLogic->checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens)) {
            return true;
        }

        return false;
    }
}