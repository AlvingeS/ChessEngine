#include "RayLogic.h"
#include "ChessEngine/bits/ChessUtils.h"

namespace movegen {

    RayLogic::RayLogic(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic) 
        : _board(board),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic) {
        _freeRayIndices.reserve(8);
    }

    void RayLogic::addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom, std::vector<game::Move>& moveList) {
        bits::getBitIndices(_freeRayIndices, freeRay);

        for (int bitIndex : _freeRayIndices) {
            _commonLogic->addMove(bitIndexFrom, bitIndex, game::Move::QUITE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, std::vector<game::Move>& moveList) {
        bool blockerIsWhite = bits::getBit(_board.getWhitePiecesBitmask(), blockerIndex);

        if (blockerIsWhite != isWhite) {
            _commonLogic->addMove(bitIndexFrom, blockerIndex, game::Move::CAPTURE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex,bool alongFile, 
                                                    bool startFromBlocker, int rookRank, 
                                                    int rookFile, 
                                                    int bitIndexFrom,
                                                    std::vector<game::Move>& moveList) {
        int start = startFromBlocker 
                    ? (alongFile ? bits::fileFromBitIndex(blockerIndex) 
                                : bits::rankFromBitIndex(blockerIndex)) 
                    : (alongFile ? rookFile 
                                : rookRank);
                                
        int stop = startFromBlocker 
                ? (alongFile ? rookFile 
                                : rookRank) 
                : (alongFile ? bits::fileFromBitIndex(blockerIndex) 
                                : bits::rankFromBitIndex(blockerIndex));

        for (int i = start - 1; i > stop; --i) {
            int rankOrFileIndex = alongFile ? rookRank * 8 + i : i * 8 + rookFile;
            _commonLogic->addMove(bitIndexFrom, rankOrFileIndex, game::Move::QUITE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, 
                                                    int bishopRank, int bishopFile, 
                                                    int bitIndexFrom,
                                                    std::vector<game::Move>& moveList) {
        
        int startRank = startFromBlocker ? bits::rankFromBitIndex(blockerIndex) : bishopRank;
        int startFile = startFromBlocker ? bits::fileFromBitIndex(blockerIndex) : bishopFile;

        int stopRank = startFromBlocker ? bishopRank : bits::rankFromBitIndex(blockerIndex);
        int stopFile = startFromBlocker ? bishopFile : bits::fileFromBitIndex(blockerIndex);

        int rankDiff = startRank - stopRank;
        int fileDiff = startFile - stopFile;

        int rankIncrement = rankDiff > 0 ? -1 : 1;
        int fileIncrement = fileDiff > 0 ? -1 : 1;

        for (int i = startRank + rankIncrement, j = startFile + fileIncrement; i != stopRank; i += rankIncrement, j += fileIncrement) {
            int rankOrFileIndex = i * 8 + j;
            _commonLogic->addMove(bitIndexFrom, rankOrFileIndex, game::Move::QUITE_FLAG, moveList, _moveIndex);
        }
    }

    void RayLogic::getMovesFromStraightRay(bits::U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<game::Move>& moveList) {
            bits::U64 blockerBitMask = ray & _board.getOccupiedPiecesBitmask();          

            if (blockerBitMask != 0) {
                int blockerIndex = blockerOnLSB ? bits::indexOfLSB(blockerBitMask) : bits::indexOfMSB(blockerBitMask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex, moveList);
                addMovesBetweenBlockerAndPieceOnStraightRay(blockerIndex, alongFile, blockerOnLSB, pieceRank, pieceFile, pieceIndex, moveList);
            } else {
                addMovesFromFreeRay(ray, pieceIndex, moveList);
            }
    }

    void RayLogic::getMovesFromDiagonalRay(bits::U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, int pieceRank, int pieceFile, std::vector<game::Move>& moveList) {
        bits::U64 blockerBitMask = ray & _board.getOccupiedPiecesBitmask();

        if (blockerBitMask != 0) {
            int blockerIndex = blockerOnLSB ? bits::indexOfLSB(blockerBitMask) : bits::indexOfMSB(blockerBitMask);
            addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex, moveList);
            addMovesBetweenBlockerAndPieceOnDiagonalRay(blockerIndex, blockerOnLSB, pieceRank, pieceFile, pieceIndex, moveList);
        } else {
            addMovesFromFreeRay(ray, pieceIndex, moveList);
        }
    }

    bool RayLogic::checkStraightRay(bits::U64& straightRay, bool firstBlockerOnLSB, bits::U64& opponentRooksAndQueens) {
        bits::U64 rooksAndQueensBlockerBitMask = straightRay & opponentRooksAndQueens;
        
        // There must be a rook or a queen on the file or rank to be in check
        if ((rooksAndQueensBlockerBitMask) != 0) {
            bits::U64 occupiedBlockerBitMask = straightRay & _board.getOccupiedPiecesBitmask();

            // If there is only one blocker out of all pieces, then it must be a rook or a queen thus the king is in check
            if (bits::popCount(occupiedBlockerBitMask) == 1) {
                return true;
            } else {
                int occupiedBlockerIndex = firstBlockerOnLSB ? bits::indexOfLSB(occupiedBlockerBitMask)
                                                             : bits::indexOfMSB(occupiedBlockerBitMask);
                int rooksAndQueensBlockerIndex = firstBlockerOnLSB ? bits::indexOfLSB(rooksAndQueensBlockerBitMask)
                                                                   : bits::indexOfMSB(rooksAndQueensBlockerBitMask);

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

    bool RayLogic::checkDiagonalRay(bits::U64& diagonalRay, bool firstBlockerOnLSB, bits::U64& opponentBishopsAndQueens) {
        bits::U64 bishopsAndQueensBlockerBitMask = diagonalRay & opponentBishopsAndQueens;

        if ((bishopsAndQueensBlockerBitMask) != 0) {
            bits::U64 occupiedBlockerBitMask = diagonalRay & _board.getOccupiedPiecesBitmask();

            if (bits::popCount(occupiedBlockerBitMask) == 1) {
                return true;
            } else {
                int occupiedBlockerIndex = firstBlockerOnLSB ? bits::indexOfLSB(occupiedBlockerBitMask)
                                                             : bits::indexOfMSB(occupiedBlockerBitMask);
                int bishopsAndQueensBlockerIndex = firstBlockerOnLSB ? bits::indexOfLSB(bishopsAndQueensBlockerBitMask)
                                                                   : bits::indexOfMSB(bishopsAndQueensBlockerBitMask);

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