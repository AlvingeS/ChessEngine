#include "MoveGenerator.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/bits/KnightBitMasks.h"
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
        _knightBitmasks = bits::getAllKnightBitMasks();
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
        genKnightMoves(isWhite);

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

    void MoveGenerator::addMovesBetweenBlockerAndPiece(int blockerIndex, bool alongFile, 
                                                    bool startFromBlocker, int rookRank, 
                                                    int rookFile, PieceType pieceType, 
                                                    int bitIndexFrom) {
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
            _moves.push_back(Move(pieceType, bitIndexFrom, rankOrFileIndex));
        }
    }

    void MoveGenerator::getMovesFromStraightRay(bits::U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, PieceType pieceType, int pieceRank, int pieceFile) {
            bits::U64 blockerBitMask = ray & _occupiedBitmask;          

            if (blockerBitMask != 0) {
                int blockerIndex = blockerOnLSB ? bits::indexOfLSB(blockerBitMask) : bits::indexOfMSB(blockerBitMask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex, pieceType);
                addMovesBetweenBlockerAndPiece(blockerIndex, alongFile, blockerOnLSB, pieceRank, pieceFile, pieceType, pieceIndex);
            } else {
                addMovesFromFreeRay(ray, pieceIndex, pieceType);
            }
    }

    void MoveGenerator::genRookMoves(bool isWhite) {
        std::vector<int> rookIndices;
        bits::StraightRays rays;

        PieceType currentPieceType = isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
        rookIndices = bits::getBitIndices(_board.getBitboard(currentPieceType));

        // Loop through all rooks and isolate them
        for (int currentRookIndex : rookIndices) {
            rays = _straightRayBitmasks[currentRookIndex];
            int rookRank = bits::rankFromBitIndex(currentRookIndex);
            int rookFile = bits::fileFromBitIndex(currentRookIndex);

            getMovesFromStraightRay(rays.north, true, false, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
            getMovesFromStraightRay(rays.east, false, true, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
            getMovesFromStraightRay(rays.south, false, false, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
            getMovesFromStraightRay(rays.west, true, true, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
        }
    }

    void MoveGenerator::genKnightMoves(bool isWhite) {
        std::vector<int> knightIndices;

        PieceType currentPieceType = isWhite ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
        knightIndices = bits::getBitIndices(_board.getBitboard(currentPieceType));

        for (int currentKnightIndex : knightIndices) {
            bits::U64 knightBitMask = _knightBitmasks[currentKnightIndex];

            bits::U64 freeKnightMoves = knightBitMask & _emptySquaresBitmask;
            bits::U64 enemyPieces = isWhite ? _blackPiecesBitmask : _whitePiecesBitmask;
            bits::U64 capturableKnightMoves = knightBitMask & enemyPieces;

            std::vector<int> freeKnightMovesIndices = bits::getBitIndices(freeKnightMoves);
            std::vector<int> capturableKnightMovesIndices = bits::getBitIndices(capturableKnightMoves);

            for (int freeKnightMoveIndex : freeKnightMovesIndices) {
                addMove(currentKnightIndex, freeKnightMoveIndex, currentPieceType);
            }

            for (int capturableKnightMoveIndex : capturableKnightMovesIndices) {
                addMove(currentKnightIndex, capturableKnightMoveIndex, currentPieceType);
            }
        }
    }
}