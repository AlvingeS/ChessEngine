#include "logic/movegen/ray_logic.h"

#include "model/move/movelist.h"
#include "model/move/move.h"

#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

void RayLogic::addMovesFromFreeRay(
    bitmask freeRay,
    int bitIndexFrom,
    model::Movelist& moveList)
{
    std::vector<int>& freeRayIndices = Containers::getSlidingPiecefreeMovesIndices();
    BitBasics::getBitIndices(freeRayIndices, freeRay);

    for (int bitIndex : freeRayIndices) {
        moveList.add_move(model::Move(bitIndexFrom, bitIndex, model::Move::QUITE_FLAG));
    }
}

void RayLogic::addMoveIfBlockerIsEnemy(
    int blockerIndex,
    bool isWhite,
    int bitIndexFrom,
    model::Movelist& moveList,
    bitmask whitePiecesBitmask)
{
    bool blockerIsWhite = BitBasics::getBit(whitePiecesBitmask, blockerIndex);

    if (blockerIsWhite != isWhite)
        moveList.add_move(model::Move(bitIndexFrom, blockerIndex, model::Move::CAPTURE_FLAG));
}

void RayLogic::addMovesBetweenBlockerAndPieceOnStraightRay(
    int blockerIndex,
    bool alongFile, 
    bool startFromBlocker, int rookRank, 
    int rookFile, 
    int bitIndexFrom,
    model::Movelist& moveList)
{
    int start = startFromBlocker 
                ? (alongFile ? ChessUtils::fileFromBitIndex(blockerIndex) 
                             : ChessUtils::rankFromBitIndex(blockerIndex)) 
                : (alongFile ? rookFile 
                             : rookRank);
                            
    int stop = startFromBlocker 
               ? (alongFile ? rookFile 
                            : rookRank) 
               : (alongFile ? ChessUtils::fileFromBitIndex(blockerIndex) 
                            : ChessUtils::rankFromBitIndex(blockerIndex));

    for (int i = start - 1; i > stop; --i) {
        int rankOrFileIndex = alongFile ? rookRank * 8 + i 
                                        : i * 8 + rookFile;
        
        moveList.add_move(model::Move(bitIndexFrom, rankOrFileIndex, model::Move::QUITE_FLAG));
    }
}

void RayLogic::addMovesBetweenBlockerAndPieceOnDiagonalRay(
    int blockerIndex,
    bool startFromBlocker, 
    int bishopRank, 
    int bishopFile, 
    int bitIndexFrom,
    model::Movelist& moveList)
{
    int startRank = startFromBlocker
                    ? ChessUtils::rankFromBitIndex(blockerIndex)
                    : bishopRank;

    int startFile = startFromBlocker
                    ? ChessUtils::fileFromBitIndex(blockerIndex)
                    : bishopFile;

    int stopRank = startFromBlocker
                   ? bishopRank
                   : ChessUtils::rankFromBitIndex(blockerIndex);

    int stopFile = startFromBlocker 
                   ? bishopFile
                   : ChessUtils::fileFromBitIndex(blockerIndex);

    int rankDiff = startRank - stopRank;
    int fileDiff = startFile - stopFile;

    int rankIncrement = rankDiff > 0 ? -1 : 1;
    int fileIncrement = fileDiff > 0 ? -1 : 1;

    for (int i = startRank + rankIncrement, j = startFile + fileIncrement; i != stopRank; i += rankIncrement, j += fileIncrement) {
        int rankOrFileIndex = i * 8 + j;

        moveList.add_move(model::Move(bitIndexFrom, rankOrFileIndex, model::Move::QUITE_FLAG));
    }
}

void RayLogic::addMovesFromStraightRay(
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
                               ? BitBasics::indexOfLSB(blockerBitmask)
                               : BitBasics::indexOfMSB(blockerBitmask);
                               
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

void RayLogic::addMovesFromDiagonalRay(
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
                           ? BitBasics::indexOfLSB(blockerBitmask) 
                           : BitBasics::indexOfMSB(blockerBitmask);

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

bool RayLogic::checkStraightRay(
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
    if (BitBasics::popCount(occupiedBlockerBitmask) == 1)
        return true;

    int occupiedBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(occupiedBlockerBitmask)
                                                 : BitBasics::indexOfMSB(occupiedBlockerBitmask);

    int rooksAndQueensBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(rooksAndQueensBlockerBitmask)
                                                       : BitBasics::indexOfMSB(rooksAndQueensBlockerBitmask);

    // If the the first blocker of any piece is the same as the first blocker of a rook or queen, then the king is in check
    return occupiedBlockerIndex == rooksAndQueensBlockerIndex;
}

bool RayLogic::checkDiagonalRay(
    bitmask diagonalRay, 
    bool firstBlockerOnLSB, 
    bitmask opponentBishopsAndQueens,
    bitmask occupiedPiecesBitmask)
{
    bitmask bishopsAndQueensBlockerBitmask = diagonalRay & opponentBishopsAndQueens;

    if ((bishopsAndQueensBlockerBitmask) == 0)
        return false;

    bitmask occupiedBlockerBitmask = diagonalRay & occupiedPiecesBitmask;

    if (BitBasics::popCount(occupiedBlockerBitmask) == 1)
        return true;

    int occupiedBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(occupiedBlockerBitmask)
                                                 : BitBasics::indexOfMSB(occupiedBlockerBitmask);

    int bishopsAndQueensBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(bishopsAndQueensBlockerBitmask)
                                                         : BitBasics::indexOfMSB(bishopsAndQueensBlockerBitmask);

    return occupiedBlockerIndex == bishopsAndQueensBlockerIndex;
}

} // namespace logic