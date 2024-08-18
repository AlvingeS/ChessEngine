#pragma once

namespace movegen {
namespace RayLogic {

    void addMovesFromStraightRay(
        bitmask ray,
        bool blockerOnLSB,
        bool alongFile,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        Movelist& moveListRef,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );

    void addMovesFromDiagonalRay(
        bitmask ray,
        bool blockerOnLSB,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        Movelist& moveListRef,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );

    bool checkStraightRay(
        bitmask straightRay,
        bool firstBlockerOnLSB,
        bitmask opponentRooksAndQueens,
        bitmask occupiedPiecesBitmask
    );

    bool checkDiagonalRay(
        bitmask diagonalRay,
        bool firstBlockerOnLSB,
        bitmask opponentBishopsAndQueens,
        bitmask occupiedPiecesBitmask
    );

} // namespace raylogic
} // namespace movegen