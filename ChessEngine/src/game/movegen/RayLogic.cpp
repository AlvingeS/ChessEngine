#include "ChessEngine/src/game/movegen/Movelist.h"

#include "ChessEngine/src/game/move/Move.h"

#include "ChessEngine/src/utils/Containers.h"
#include "ChessEngine/src/utils/ChessUtils.h"
#include "ChessEngine/src/utils/BitBasics.h"

namespace game {
namespace movegen {
namespace RayLogic {

void addMovesFromFreeRay(
    bitmask freeRay,
    int bitIndexFrom,
    Movelist& moveListRef)
{
    std::vector<int>& freeRayIndices = utils::Containers::getSlidingPiecefreeMovesIndices();
    utils::getBitIndices(freeRayIndices, freeRay);

    for (int bitIndex : freeRayIndices) {
        moveListRef.addMove(move::Move(bitIndexFrom, bitIndex, move::Move::QUITE_FLAG));
    }
}

void addMoveIfBlockerIsEnemy(
    int blockerIndex,
    bool isWhite,
    int bitIndexFrom,
    Movelist& moveListRef,
    bitmask whitePiecesBitmask)
{
    bool blockerIsWhite = utils::getBit(whitePiecesBitmask, blockerIndex);

    if (blockerIsWhite != isWhite) {
        moveListRef.addMove(move::Move(bitIndexFrom, blockerIndex, move::Move::CAPTURE_FLAG));
    }
}

void addMovesBetweenBlockerAndPieceOnStraightRay(
    int blockerIndex,
    bool alongFile, 
    bool startFromBlocker, int rookRank, 
    int rookFile, 
    int bitIndexFrom,
    Movelist& moveListRef)
{
    int start = startFromBlocker 
                ? (alongFile ? utils::fileFromBitIndex(blockerIndex) 
                             : utils::rankFromBitIndex(blockerIndex)) 
                : (alongFile ? rookFile 
                             : rookRank);
                            
    int stop = startFromBlocker 
               ? (alongFile ? rookFile 
                            : rookRank) 
               : (alongFile ? utils::fileFromBitIndex(blockerIndex) 
                            : utils::rankFromBitIndex(blockerIndex));

    for (int i = start - 1; i > stop; --i) {
        int rankOrFileIndex = alongFile ? rookRank * 8 + i : i * 8 + rookFile;
        
        moveListRef.addMove(move::Move(bitIndexFrom, rankOrFileIndex, move::Move::QUITE_FLAG));
    }
}

void addMovesBetweenBlockerAndPieceOnDiagonalRay(
    int blockerIndex,
    bool startFromBlocker, 
    int bishopRank, 
    int bishopFile, 
    int bitIndexFrom,
    Movelist& moveListRef)
{
    int startRank = startFromBlocker
                    ? utils::rankFromBitIndex(blockerIndex)
                    : bishopRank;

    int startFile = startFromBlocker
                    ? utils::fileFromBitIndex(blockerIndex)
                    : bishopFile;

    int stopRank = startFromBlocker
                   ? bishopRank
                   : utils::rankFromBitIndex(blockerIndex);

    int stopFile = startFromBlocker 
                   ? bishopFile
                   : utils::fileFromBitIndex(blockerIndex);

    int rankDiff = startRank - stopRank;
    int fileDiff = startFile - stopFile;

    int rankIncrement = rankDiff > 0 ? -1 : 1;
    int fileIncrement = fileDiff > 0 ? -1 : 1;

    for (int i = startRank + rankIncrement, j = startFile + fileIncrement; i != stopRank; i += rankIncrement, j += fileIncrement) {
        int rankOrFileIndex = i * 8 + j;

        moveListRef.addMove(move::Move(bitIndexFrom, rankOrFileIndex, move::Move::QUITE_FLAG));
    }
}

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
    bitmask occupiedPiecesBitmask) 
{
        bitmask blockerBitmask = ray & occupiedPiecesBitmask;          

        if (blockerBitmask != 0) {
            int blockerIndex = blockerOnLSB
                               ? utils::indexOfLSB(blockerBitmask)
                               : utils::indexOfMSB(blockerBitmask);
                               
            addMoveIfBlockerIsEnemy(
                blockerIndex,
                isWhite,
                pieceIndex, 
                moveListRef,
                whitePiecesBitmask
            );

            addMovesBetweenBlockerAndPieceOnStraightRay(
                blockerIndex,
                alongFile, 
                blockerOnLSB, 
                pieceRank, 
                pieceFile, 
                pieceIndex, 
                moveListRef
            );

        } else {
            addMovesFromFreeRay(ray, pieceIndex, moveListRef);
        }
}

void addMovesFromDiagonalRay(
    bitmask ray, 
    bool blockerOnLSB, 
    bool isWhite, 
    int pieceIndex, 
    int pieceRank, 
    int pieceFile, 
    Movelist& moveListRef,
    bitmask whitePiecesBitmask,
    bitmask occupiedPiecesBitmask)
{
    bitmask blockerBitmask = ray & occupiedPiecesBitmask;

    if (blockerBitmask != 0) {
        int blockerIndex = blockerOnLSB
                           ? utils::indexOfLSB(blockerBitmask) 
                           : utils::indexOfMSB(blockerBitmask);

        addMoveIfBlockerIsEnemy(
            blockerIndex, 
            isWhite, 
            pieceIndex, 
            moveListRef,
            whitePiecesBitmask
        );

        addMovesBetweenBlockerAndPieceOnDiagonalRay(
            blockerIndex, 
            blockerOnLSB,
            pieceRank, 
            pieceFile, 
            pieceIndex, 
            moveListRef
        );

    } else {
        addMovesFromFreeRay(ray, pieceIndex, moveListRef);
    }
}

bool checkStraightRay(
    bitmask straightRay, 
    bool firstBlockerOnLSB, 
    bitmask opponentRooksAndQueens,
    bitmask occupiedPiecesBitmask) 
{
    bitmask rooksAndQueensBlockerBitmask = straightRay & opponentRooksAndQueens;
    
    // There must be a rook or a queen on the file or rank to be in check
    if (rooksAndQueensBlockerBitmask != 0ULL) {
        bitmask occupiedBlockerBitmask = straightRay & occupiedPiecesBitmask;

        // If there is only one blocker out of all pieces, then it must be a rook or a queen thus the king is in check
        if (utils::popCount(occupiedBlockerBitmask) == 1) {
            return true;
        } else {
            int occupiedBlockerIndex = firstBlockerOnLSB
                                       ? utils::indexOfLSB(occupiedBlockerBitmask)
                                       : utils::indexOfMSB(occupiedBlockerBitmask);

            int rooksAndQueensBlockerIndex = firstBlockerOnLSB 
                                             ? utils::indexOfLSB(rooksAndQueensBlockerBitmask)
                                             : utils::indexOfMSB(rooksAndQueensBlockerBitmask);

            // If the the first blocker of any piece is the same as the first blocker of a rook or queen, then the king is in check
            if (occupiedBlockerIndex == rooksAndQueensBlockerIndex) {
                return true;
            } else {
                return false;
            }
        }
    }

    return false;
}

bool checkDiagonalRay(
    bitmask diagonalRay, 
    bool firstBlockerOnLSB, 
    bitmask opponentBishopsAndQueens,
    bitmask occupiedPiecesBitmask)
{
    bitmask bishopsAndQueensBlockerBitmask = diagonalRay & opponentBishopsAndQueens;

    if ((bishopsAndQueensBlockerBitmask) != 0) {
        bitmask occupiedBlockerBitmask = diagonalRay & occupiedPiecesBitmask;

        if (utils::popCount(occupiedBlockerBitmask) == 1) {
            return true;
        } else {
            int occupiedBlockerIndex = firstBlockerOnLSB
                                       ? utils::indexOfLSB(occupiedBlockerBitmask)
                                       : utils::indexOfMSB(occupiedBlockerBitmask);

            int bishopsAndQueensBlockerIndex = firstBlockerOnLSB
                                               ? utils::indexOfLSB(bishopsAndQueensBlockerBitmask)
                                               : utils::indexOfMSB(bishopsAndQueensBlockerBitmask);

            if (occupiedBlockerIndex == bishopsAndQueensBlockerIndex) {
                return true;
            } else {
                return false;
            }
        }
    }

    return false;
}

} // namespace raylogic
} // namespace movegen
} // namespace game