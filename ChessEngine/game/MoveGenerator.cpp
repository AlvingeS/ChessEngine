#include "MoveGenerator.h"
#include "ChessEngine/bits/BitMasks.h"
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
        _moves = std::vector<Move>(MAX_LEGAL_MOVES);
        _board = ChessBoard();
        _straightRayBitMasks = bits::getAllStraightRayBitMasks();
        _diagonalRayBitMasks = bits::getAllDiagonalRayBitMasks();
        _whiteSquaresBitMask = bits::getWhiteSquaresBitMask();
        _blackSquaresBitMask = bits::getBlackSquaresBitMask();
        updateGameStateBitMasks();
    }

    void MoveGenerator::updateGameStateBitMasks() {
        _whitePiecesBitMask = _board.getWhitePiecesBitMask();
        _blackPiecesBitMask = _board.getBlackPiecesBitMask();
        _occupiedBitMask = bits::getOccupiedSquaresBitMask(_whitePiecesBitMask, _blackPiecesBitMask);
        _emptySquaresBitMask = bits::getEmptySquaresBitmask(_whitePiecesBitMask, _blackPiecesBitMask);
    }

    void MoveGenerator::setBoardFromFen(std::string fen) {
        _board.setBoardFromFen(fen);
        updateGameStateBitMasks();
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
            _moves.push_back(Move(pieceType, bitIndexFrom, bitIndex));
        }
    }

    bool MoveGenerator::processRay(bits::U64 ray, int pieceIndex, PieceType pieceType, bool isWhite, int blockerIndex) {
    bits::U64 blockerMask = ray & _occupiedBitMask;
    if (blockerMask == 0) {
        addMovesFromFreeRay(ray, pieceIndex, pieceType);
        return false;
    } else {
        bool blockerIsWhite = bits::getBit(_whitePiecesBitMask, blockerIndex);

        if (blockerIsWhite != isWhite) {
            _moves.push_back(Move(pieceType, pieceIndex, blockerIndex));
        }
    }

    return true;
}

    void MoveGenerator::genRookMoves(bool isWhite) {
        std::vector<int> rookIndices;
        bits::StraightRays rays;
        std::vector<int> bitIndicesFreeRay;

        PieceType currentPieceType = isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
        rookIndices = bits::getBitIndices(_board.getBitboard(currentPieceType));

        // Loop through all rooks and isolate them
        for (int currentRookIndex : rookIndices) {
            rays = _straightRayBitMasks[currentRookIndex];
            int rookRank = bits::rankFromBitIndex(currentRookIndex);
            int rookFile = bits::fileFromBitIndex(currentRookIndex);
            int blockerIndex;
            int blockerRank;
            int blockerFile;
            bool blockerFound;

            // Handle north ray. If the ray is free, add all moves from it.
            blockerIndex = bits::indexOfLSB(rays.north & _occupiedBitMask);
            blockerFound  = processRay(rays.north, currentRookIndex, currentPieceType, isWhite, blockerIndex);

            if (blockerFound) {
                blockerRank = bits::rankFromBitIndex(blockerIndex);

                for (int i = blockerRank - 1; i > rookRank; i--) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, i * 8 + rookFile));
                }
            }
            
            // Handle east ray. If the ray is free, add all moves from it.
            blockerIndex = bits::indexOfMSB(rays.east & _occupiedBitMask);
            blockerFound  = processRay(rays.east, currentRookIndex, currentPieceType, isWhite, blockerIndex);

            if (blockerFound) {
                blockerFile = bits::fileFromBitIndex(blockerIndex);

                for (int i = blockerFile + 1; i < rookFile; i++) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, rookRank * 8 + i));
                }
            }


            // Handle south ray. If the ray is free, add all moves from it.
            blockerIndex = bits::indexOfMSB(rays.south & _occupiedBitMask);
            blockerFound  = processRay(rays.south, currentRookIndex, currentPieceType, isWhite, blockerIndex);


            if (blockerFound) {
                blockerRank = bits::rankFromBitIndex(blockerIndex);

                for (int i = blockerRank + 1; i < rookRank; i++) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, i * 8 + rookFile));
                }
            }

            // Handle west ray. If the ray is free, add all moves from it.
            blockerIndex = bits::indexOfLSB(rays.west & _occupiedBitMask);
            blockerFound = processRay(rays.west, currentRookIndex, currentPieceType, isWhite, blockerIndex);

            if (blockerFound) {
                blockerFile = bits::fileFromBitIndex(blockerIndex);

                for (int i = blockerFile - 1; i > rookFile; i--) {
                    _moves.push_back(Move(currentPieceType, currentRookIndex, rookRank * 8 + i));
                }
            }
        }
    }
}