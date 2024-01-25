#include "MoveGenerator.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/BitUtils.h"
#include <iostream>

/*
IDEA FOR CHECK-CHECKING

1. Imagine that the king is your only piece on the board.
2. Generate all moves for the king, raycast as if it was a rook.
3. If the rays hits sliding pieces, check if the enemy pieces are rooks or queens.
4. If that is the case, then check what what pieces you own along that ray, blocking the attack.
5. Save the ray, and tag the piece or pieces as blockers (perhaps in a bitboard). These pieces can now only move along the ray.
6. Repeat for the king as if it was a bishop.

Now when calculating moves that is not the king, you can check wheter or not a piece is a blocker,
and if it is, you can only move along the ray that it is blocking. Otherwise you do not
have to check for check.

For the king, perform a queens raycast after each move but with your own pieces as well.

I have to figure out what to do if I am actually in check, and there are no blockers.
I realized, that it is the first thing to check actually. If I am in check, I can only
attempt to find moves that could block the check, move the king or eliminate the threat.
*/


namespace game {
    MoveGenerator::MoveGenerator() {
        _moves.reserve(MAX_LEGAL_MOVES);
        _board = ChessBoard();
        _straightRayBitmasks = bits::getAllStraightRayBitmasks();
        _diagonalRayBitmasks = bits::getAllDiagonalRayBitmasks();
        _whiteSquaresBitmask = bits::getWhiteSquaresBitmask();
        _blackSquaresBitmask = bits::getBlackSquaresBitmask();
        updateGameStateBitmasks();
    }

    void MoveGenerator::updateGameStateBitmasks() {
        _whitePiecesBitmask = _board.getWhitePiecesBitmask();
        _blackPiecesBitmask = _board.getBlackPiecesBitmask();
        _occupiedBitmask = bits::getOccupiedSquaresBitmask(_whitePiecesBitmask, _blackPiecesBitmask);
        _emptySquaresBitmask = bits::getEmptySquaresBitmask(_whitePiecesBitmask, _blackPiecesBitmask);
    }

    void MoveGenerator::setBoardFromFen(std::string fen) {
        _board.setBoardFromFen(fen);
        updateGameStateBitmasks();
    }

    void MoveGenerator::addMove(int bitIndexFrom, int bitIndexTo, PieceType pieceType) {
        _moves.push_back(Move(pieceType, bitIndexFrom, bitIndexTo));
    }

    // count the number of elements in a vector that are not null
    int MoveGenerator::getNumLegalMoves() {
        int numLegalMoves = 0;

        for (Move move : _moves) {
            if (move.getPieceType() != PieceType::ERROR) {
                numLegalMoves++;
            }
        }

        return numLegalMoves;
    }

    std::vector<Move>& MoveGenerator::genMoves(bool isWhite) {
        genRookMoves(isWhite);

        return _moves;
    }

    void MoveGenerator::addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom, PieceType pieceType) {
        std::vector<int> bitIndicesFreeRay = bits::getBitIndices(freeRay);

        for (int bitIndex : bitIndicesFreeRay) {
            addMove(bitIndexFrom, bitIndex, pieceType);
        }
    }

    void MoveGenerator::addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, PieceType pieceType) {
        bool blockerIsWhite = bits::getBit(_whitePiecesBitmask, blockerIndex);

        if (blockerIsWhite != isWhite) {
            addMove(bitIndexFrom, blockerIndex, pieceType);
        }
    }

    void MoveGenerator::genRookMoves(bool isWhite) {
        std::vector<int> rookIndices;
        bits::StraightRays rays;
        std::vector<int> bitIndicesFreeRay;

        PieceType currentPieceType = isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
        rookIndices = bits::getBitIndices(_board.getBitboard(currentPieceType));

        // Loop through all rooks and isolate them
        for (int currentRookIndex : rookIndices) {
            rays = _straightRayBitmasks[currentRookIndex];
            int rookRank = bits::rankFromBitIndex(currentRookIndex);
            int rookFile = bits::fileFromBitIndex(currentRookIndex);
            int blockerIndex;
            int blockerRank;
            int blockerFile;
            bool blockerFound;

            // North ray
            blockerFound = (rays.north & _occupiedBitmask) != 0;

            if (blockerFound) {
                blockerIndex = bits::indexOfLSB(rays.north & _occupiedBitmask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, currentRookIndex, currentPieceType);
                
                blockerRank = bits::rankFromBitIndex(blockerIndex);

                for (int i = blockerRank - 1; i > rookRank; i--) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, i * 8 + rookFile));
                }
            } else {
                addMovesFromFreeRay(rays.north, currentRookIndex, currentPieceType);
            }

            // East ray
            blockerFound  = (rays.east & _occupiedBitmask) != 0;
            
            if (blockerFound) {
                blockerIndex = bits::indexOfMSB(rays.east & _occupiedBitmask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, currentRookIndex, currentPieceType);

                blockerFile = bits::fileFromBitIndex(blockerIndex);

                for (int i = blockerFile + 1; i < rookFile; i++) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, rookRank * 8 + i));
                }
            } else {
                addMovesFromFreeRay(rays.east, currentRookIndex, currentPieceType);
            }

            // South ray
            blockerFound  = (rays.south & _occupiedBitmask) != 0;
            
            if (blockerFound) {
                blockerIndex = bits::indexOfMSB(rays.south & _occupiedBitmask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, currentRookIndex, currentPieceType);

                blockerRank = bits::rankFromBitIndex(blockerIndex);

                for (int i = blockerRank + 1; i < rookRank; i++) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, i * 8 + rookFile));
                }
            } else {
                addMovesFromFreeRay(rays.south, currentRookIndex, currentPieceType);
            }

            // West ray
            blockerFound  = (rays.west & _occupiedBitmask) != 0;
            if (blockerFound) {
                blockerIndex = bits::indexOfLSB(rays.west & _occupiedBitmask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, currentRookIndex, currentPieceType);

                blockerFile = bits::fileFromBitIndex(blockerIndex);

                for (int i = blockerFile - 1; i > rookFile; i--) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, rookRank * 8 + i));
                }
            } else {
                addMovesFromFreeRay(rays.west, currentRookIndex, currentPieceType);
            }
        }
    }
}