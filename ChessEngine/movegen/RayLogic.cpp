#include "RayLogic.h"

#include "ChessEngine/utils/ChessUtils.h"

namespace movegen {

    RayLogic::RayLogic(board::GameStateBitMasks& gameStateBitmasks, int& moveIndex, CommonLogic* commonLogic) 
        : _gameStateBitMasks(gameStateBitmasks),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic) {
        _freeRayIndices.reserve(8);
    }

    void RayLogic::addMovesFromFreeRay(U64 freeRay, int bitIndexFrom, std::vector<move::Move>& moveList) {
        utils::getBitIndices(_freeRayIndices, freeRay);

        for (int bitIndex : _freeRayIndices) {
            _commonLogic->addMove(bitIndexFrom, bitIndex, move::Move::QUITE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, std::vector<move::Move>& moveList) {
        bool blockerIsWhite = utils::getBit(_gameStateBitMasks.getWhitePiecesBitmask(), blockerIndex);

        if (blockerIsWhite != isWhite) {
            _commonLogic->addMove(bitIndexFrom, blockerIndex, move::Move::CAPTURE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex,bool alongFile, 
                                                    bool startFromBlocker, int rookRank, 
                                                    int rookFile, 
                                                    int bitIndexFrom,
                                                    std::vector<move::Move>& moveList) {
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
            _commonLogic->addMove(bitIndexFrom, rankOrFileIndex, move::Move::QUITE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, 
                                                    int bishopRank, int bishopFile, 
                                                    int bitIndexFrom,
                                                    std::vector<move::Move>& moveList) {
        
        int startRank = startFromBlocker ? utils::rankFromBitIndex(blockerIndex) : bishopRank;
        int startFile = startFromBlocker ? utils::fileFromBitIndex(blockerIndex) : bishopFile;

        int stopRank = startFromBlocker ? bishopRank : utils::rankFromBitIndex(blockerIndex);
        int stopFile = startFromBlocker ? bishopFile : utils::fileFromBitIndex(blockerIndex);

        int rankDiff = startRank - stopRank;
        int fileDiff = startFile - stopFile;

        int rankIncrement = rankDiff > 0 ? -1 : 1;
        int fileIncrement = fileDiff > 0 ? -1 : 1;

        for (int i = startRank + rankIncrement, j = startFile + fileIncrement; i != stopRank; i += rankIncrement, j += fileIncrement) {
            int rankOrFileIndex = i * 8 + j;
            _commonLogic->addMove(bitIndexFrom, rankOrFileIndex, move::Move::QUITE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::getMovesFromStraightRay(U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<move::Move>& moveList) {
            U64 blockerBitMask = ray & _gameStateBitMasks.getOccupiedPiecesBitmask();          

            if (blockerBitMask != 0) {
                int blockerIndex = blockerOnLSB ? utils::indexOfLSB(blockerBitMask) : utils::indexOfMSB(blockerBitMask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex, moveList);
                addMovesBetweenBlockerAndPieceOnStraightRay(blockerIndex, alongFile, blockerOnLSB, pieceRank, pieceFile, pieceIndex, moveList);
            } else {
                addMovesFromFreeRay(ray, pieceIndex, moveList);
            }
    }

    void RayLogic::getMovesFromDiagonalRay(U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<move::Move>& moveList) {
        U64 blockerBitMask = ray & _gameStateBitMasks.getOccupiedPiecesBitmask();

        if (blockerBitMask != 0) {
            int blockerIndex = blockerOnLSB ? utils::indexOfLSB(blockerBitMask) : utils::indexOfMSB(blockerBitMask);
            addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex, moveList);
            addMovesBetweenBlockerAndPieceOnDiagonalRay(blockerIndex, blockerOnLSB, pieceRank, pieceFile, pieceIndex, moveList);
        } else {
            addMovesFromFreeRay(ray, pieceIndex, moveList);
        }
    }

    bool RayLogic::checkStraightRay(U64& straightRay, bool firstBlockerOnLSB, U64& opponentRooksAndQueens) {
        U64 rooksAndQueensBlockerBitMask = straightRay & opponentRooksAndQueens;
        
        // There must be a rook or a queen on the file or rank to be in check
        if (rooksAndQueensBlockerBitMask != 0ULL) {
            U64 occupiedBlockerBitMask = straightRay & _gameStateBitMasks.getOccupiedPiecesBitmask();

            // If there is only one blocker out of all pieces, then it must be a rook or a queen thus the king is in check
            if (utils::popCount(occupiedBlockerBitMask) == 1) {
                return true;
            } else {
                int occupiedBlockerIndex = firstBlockerOnLSB ? utils::indexOfLSB(occupiedBlockerBitMask)
                                                             : utils::indexOfMSB(occupiedBlockerBitMask);
                int rooksAndQueensBlockerIndex = firstBlockerOnLSB ? utils::indexOfLSB(rooksAndQueensBlockerBitMask)
                                                                   : utils::indexOfMSB(rooksAndQueensBlockerBitMask);

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

    bool RayLogic::checkDiagonalRay(U64& diagonalRay, bool firstBlockerOnLSB, U64& opponentBishopsAndQueens) {
        U64 bishopsAndQueensBlockerBitMask = diagonalRay & opponentBishopsAndQueens;

        if ((bishopsAndQueensBlockerBitMask) != 0) {
            U64 occupiedBlockerBitMask = diagonalRay & _gameStateBitMasks.getOccupiedPiecesBitmask();

            if (utils::popCount(occupiedBlockerBitMask) == 1) {
                return true;
            } else {
                int occupiedBlockerIndex = firstBlockerOnLSB ? utils::indexOfLSB(occupiedBlockerBitMask)
                                                             : utils::indexOfMSB(occupiedBlockerBitMask);
                int bishopsAndQueensBlockerIndex = firstBlockerOnLSB ? utils::indexOfLSB(bishopsAndQueensBlockerBitMask)
                                                                   : utils::indexOfMSB(bishopsAndQueensBlockerBitMask);

                if (occupiedBlockerIndex == bishopsAndQueensBlockerIndex) {
                    return true;
                } else {
                    return false;
                }
            }
        }

        return false;
    }
}