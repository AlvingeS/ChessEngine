#include "ChessEngine/src/model/move/Movelist.h"

#include "ChessEngine/src/model/move/Move.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

void addMovesFromFreeRay(
    bitmask freeRay,
    int bitIndexFrom,
    model::Movelist& moveList)
{
    std::vector<int>& freeRayIndices = Containers::getSlidingPiecefreeMovesIndices();
    getBitIndices(freeRayIndices, freeRay);

    for (int bitIndex : freeRayIndices) {
        moveList.addMove(model::Move(bitIndexFrom, bitIndex, model::Move::QUITE_FLAG));
    }
}

void addMoveIfBlockerIsEnemy(
    int blockerIndex,
    bool isWhite,
    int bitIndexFrom,
    model::Movelist& moveList,
    bitmask whitePiecesBitmask)
{
    bool blockerIsWhite = getBit(whitePiecesBitmask, blockerIndex);

    if (blockerIsWhite != isWhite)
        moveList.addMove(model::Move(bitIndexFrom, blockerIndex, model::Move::CAPTURE_FLAG));
}

void addMovesBetweenBlockerAndPieceOnStraightRay(
    int blockerIndex,
    bool alongFile, 
    bool startFromBlocker, int rookRank, 
    int rookFile, 
    int bitIndexFrom,
    model::Movelist& moveList)
{
    int start = startFromBlocker 
                ? (alongFile ? fileFromBitIndex(blockerIndex) 
                             : rankFromBitIndex(blockerIndex)) 
                : (alongFile ? rookFile 
                             : rookRank);
                            
    int stop = startFromBlocker 
               ? (alongFile ? rookFile 
                            : rookRank) 
               : (alongFile ? fileFromBitIndex(blockerIndex) 
                            : rankFromBitIndex(blockerIndex));

    for (int i = start - 1; i > stop; --i) {
        int rankOrFileIndex = alongFile ? rookRank * 8 + i 
                                        : i * 8 + rookFile;
        
        moveList.addMove(model::Move(bitIndexFrom, rankOrFileIndex, model::Move::QUITE_FLAG));
    }
}

void addMovesBetweenBlockerAndPieceOnDiagonalRay(
    int blockerIndex,
    bool startFromBlocker, 
    int bishopRank, 
    int bishopFile, 
    int bitIndexFrom,
    model::Movelist& moveList)
{
    int startRank = startFromBlocker
                    ? rankFromBitIndex(blockerIndex)
                    : bishopRank;

    int startFile = startFromBlocker
                    ? fileFromBitIndex(blockerIndex)
                    : bishopFile;

    int stopRank = startFromBlocker
                   ? bishopRank
                   : rankFromBitIndex(blockerIndex);

    int stopFile = startFromBlocker 
                   ? bishopFile
                   : fileFromBitIndex(blockerIndex);

    int rankDiff = startRank - stopRank;
    int fileDiff = startFile - stopFile;

    int rankIncrement = rankDiff > 0 ? -1 : 1;
    int fileIncrement = fileDiff > 0 ? -1 : 1;

    for (int i = startRank + rankIncrement, j = startFile + fileIncrement; i != stopRank; i += rankIncrement, j += fileIncrement) {
        int rankOrFileIndex = i * 8 + j;

        moveList.addMove(model::Move(bitIndexFrom, rankOrFileIndex, model::Move::QUITE_FLAG));
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
    model::Movelist& moveList,
    bitmask whitePiecesBitmask,
    bitmask occupiedPiecesBitmask) 
{
        bitmask blockerBitmask = ray & occupiedPiecesBitmask;          

        if (blockerBitmask != 0) {
            int blockerIndex = blockerOnLSB
                               ? indexOfLSB(blockerBitmask)
                               : indexOfMSB(blockerBitmask);
                               
            addMoveIfBlockerIsEnemy(
                blockerIndex,
                isWhite,
                pieceIndex, 
                moveList,
                whitePiecesBitmask
            );

            addMovesBetweenBlockerAndPieceOnStraightRay(
                blockerIndex,
                alongFile, 
                blockerOnLSB, 
                pieceRank, 
                pieceFile, 
                pieceIndex, 
                moveList
            );

        } else {
            addMovesFromFreeRay(ray, pieceIndex, moveList);
        }
}

void addMovesFromDiagonalRay(
    bitmask ray, 
    bool blockerOnLSB, 
    bool isWhite, 
    int pieceIndex, 
    int pieceRank, 
    int pieceFile, 
    model::Movelist& moveList,
    bitmask whitePiecesBitmask,
    bitmask occupiedPiecesBitmask)
{
    bitmask blockerBitmask = ray & occupiedPiecesBitmask;

    if (blockerBitmask != 0) {
        int blockerIndex = blockerOnLSB
                           ? indexOfLSB(blockerBitmask) 
                           : indexOfMSB(blockerBitmask);

        addMoveIfBlockerIsEnemy(
            blockerIndex, 
            isWhite, 
            pieceIndex, 
            moveList,
            whitePiecesBitmask
        );

        addMovesBetweenBlockerAndPieceOnDiagonalRay(
            blockerIndex, 
            blockerOnLSB,
            pieceRank, 
            pieceFile, 
            pieceIndex, 
            moveList
        );

    } else {
        addMovesFromFreeRay(ray, pieceIndex, moveList);
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
    if (rooksAndQueensBlockerBitmask == 0ULL)
        return false;

    bitmask occupiedBlockerBitmask = straightRay & occupiedPiecesBitmask;

    // If there is only one blocker out of all pieces, then it must be a rook or a queen thus the king is in check
    if (popCount(occupiedBlockerBitmask) == 1)
        return true;

    int occupiedBlockerIndex = firstBlockerOnLSB ? indexOfLSB(occupiedBlockerBitmask)
                                                 : indexOfMSB(occupiedBlockerBitmask);

    int rooksAndQueensBlockerIndex = firstBlockerOnLSB ? indexOfLSB(rooksAndQueensBlockerBitmask)
                                                       : indexOfMSB(rooksAndQueensBlockerBitmask);

    // If the the first blocker of any piece is the same as the first blocker of a rook or queen, then the king is in check
    return occupiedBlockerIndex == rooksAndQueensBlockerIndex;
}

bool checkDiagonalRay(
    bitmask diagonalRay, 
    bool firstBlockerOnLSB, 
    bitmask opponentBishopsAndQueens,
    bitmask occupiedPiecesBitmask)
{
    bitmask bishopsAndQueensBlockerBitmask = diagonalRay & opponentBishopsAndQueens;

    if ((bishopsAndQueensBlockerBitmask) == 0)
        return false;

    bitmask occupiedBlockerBitmask = diagonalRay & occupiedPiecesBitmask;

    if (popCount(occupiedBlockerBitmask) == 1)
        return true;

    int occupiedBlockerIndex = firstBlockerOnLSB ? indexOfLSB(occupiedBlockerBitmask)
                                                 : indexOfMSB(occupiedBlockerBitmask);

    int bishopsAndQueensBlockerIndex = firstBlockerOnLSB ? indexOfLSB(bishopsAndQueensBlockerBitmask)
                                                         : indexOfMSB(bishopsAndQueensBlockerBitmask);

    return occupiedBlockerIndex == bishopsAndQueensBlockerIndex;
}

} // namespace logic