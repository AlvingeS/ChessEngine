#include "MoveGenerator.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/bits/KnightBitMasks.h"
#include "ChessEngine/bits/KingBitMasks.h"
#include "ChessEngine/bits/PawnBitMasks.h"
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
        _freeRayIndices.reserve(8);
        _freeMovesIndices.reserve(8);
        _capturableMovesIndices.reserve(8);

        _rookIndices.reserve(64);
        _bishopIndices.reserve(64);
        _queenIndices.reserve(64);
        _knightIndices.reserve(64);
        //TODO: Remove king vector
        _kingIndices.reserve(64); 
        _pawnIndices.reserve(64);

        _board = ChessBoard();
        _straightRayBitmasks = bits::getAllStraightRayBitmasks();
        _diagonalRayBitmasks = bits::getAllDiagonalRayBitmasks();
        _whiteSquaresBitmask = bits::getWhiteSquaresBitmask();
        _blackSquaresBitmask = bits::getBlackSquaresBitmask();
        _knightBitmasks = bits::getAllKnightBitMasks();
        _kingBitmasks = bits::getAllKingBitMasks();
        _whitePawnStraightMoveBitmasks = bits::getAllStraightPawnMoveBitmasks(true);
        _whitePawnCaptureMoveBitmasks = bits::getAllCapturePawnMoveBitmasks(true);
        _blackPawnStraightMoveBitmasks = bits::getAllStraightPawnMoveBitmasks(false);
        _blackPawnCaptureMoveBitmasks = bits::getAllCapturePawnMoveBitmasks(false);

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
        _moves[_moveIndex] = Move(pieceType, bitIndexFrom, bitIndexTo);
        _moveIndex++;
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
        genBishopMoves(isWhite);
        genQueenMoves(isWhite);
        genKingMoves(isWhite);
        genPawnMoves(isWhite);

        return _moves;
    }

    void MoveGenerator::resetMoves() {
        _moves.clear();
        _moveIndex = 0;
    }

    void MoveGenerator::addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom, PieceType pieceType) {
        bits::getBitIndices(_freeRayIndices, freeRay);

        for (int bitIndex : _freeRayIndices) {
            addMove(bitIndexFrom, bitIndex, pieceType);
        }
    }

    void MoveGenerator::addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, PieceType pieceType) {
        bool blockerIsWhite = bits::getBit(_whitePiecesBitmask, blockerIndex);

        if (blockerIsWhite != isWhite) {
            addMove(bitIndexFrom, blockerIndex, pieceType);
        }
    }

    void MoveGenerator::addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex, bool alongFile, 
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
            addMove(bitIndexFrom, rankOrFileIndex, pieceType);
        }
    }

    void MoveGenerator::addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, 
                                                    int bishopRank, int bishopFile, 
                                                    PieceType pieceType, int bitIndexFrom) {
        
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
            addMove(bitIndexFrom, rankOrFileIndex, pieceType);
        }
    }

    void MoveGenerator::getMovesFromStraightRay(bits::U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, PieceType pieceType, int pieceRank, int pieceFile) {
            bits::U64 blockerBitMask = ray & _occupiedBitmask;          

            if (blockerBitMask != 0) {
                int blockerIndex = blockerOnLSB ? bits::indexOfLSB(blockerBitMask) : bits::indexOfMSB(blockerBitMask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex, pieceType);
                addMovesBetweenBlockerAndPieceOnStraightRay(blockerIndex, alongFile, blockerOnLSB, pieceRank, pieceFile, pieceType, pieceIndex);
            } else {
                addMovesFromFreeRay(ray, pieceIndex, pieceType);
            }
    }

    void MoveGenerator::getMovesFromDiagonalRay(bits::U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, PieceType pieceType, int pieceRank, int pieceFile) {
        bits::U64 blockerBitMask = ray & _occupiedBitmask;

        if (blockerBitMask != 0) {
            int blockerIndex = blockerOnLSB ? bits::indexOfLSB(blockerBitMask) : bits::indexOfMSB(blockerBitMask);
            addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex, pieceType);
            addMovesBetweenBlockerAndPieceOnDiagonalRay(blockerIndex, blockerOnLSB, pieceRank, pieceFile, pieceType, pieceIndex);
        } else {
            addMovesFromFreeRay(ray, pieceIndex, pieceType);
        }
    }

    void MoveGenerator::genRookMoves(bool isWhite) {
        bits::StraightRays rays;

        PieceType currentPieceType = isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
        bits::getBitIndices(_rookIndices, _board.getBitboard(currentPieceType));

        // Loop through all rooks and isolate them
        for (int currentRookIndex : _rookIndices) {
            rays = _straightRayBitmasks[currentRookIndex];
            int rookRank = bits::rankFromBitIndex(currentRookIndex);
            int rookFile = bits::fileFromBitIndex(currentRookIndex);

            getMovesFromStraightRay(rays.north, true, false, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
            getMovesFromStraightRay(rays.east, false, true, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
            getMovesFromStraightRay(rays.south, false, false, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
            getMovesFromStraightRay(rays.west, true, true, isWhite, currentRookIndex, currentPieceType, rookRank, rookFile);
        }
    }

    void MoveGenerator::genBishopMoves(bool isWhite) {
        PieceType currentPieceType = isWhite ? PieceType::W_BISHOP : PieceType::B_BISHOP;
        bits::getBitIndices(_bishopIndices, _board.getBitboard(currentPieceType));

        for (int currentBishopIndex : _bishopIndices) {
            bits::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
            int bishopRank = bits::rankFromBitIndex(currentBishopIndex);
            int bishopFile = bits::fileFromBitIndex(currentBishopIndex);

            getMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, currentPieceType, bishopRank, bishopFile);
            getMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, currentPieceType, bishopRank, bishopFile);
            getMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, currentPieceType, bishopRank, bishopFile);
            getMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, currentPieceType, bishopRank, bishopFile);
        }
    }

    void MoveGenerator::genKnightMoves(bool isWhite) {
        PieceType currentPieceType = isWhite ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
        bits::getBitIndices(_knightIndices, _board.getBitboard(currentPieceType));

        for (int currentKnightIndex : _knightIndices) {
            bits::U64 knightBitMask = _knightBitmasks[currentKnightIndex];

            bits::U64 freeKnightMoves = knightBitMask & _emptySquaresBitmask;
            bits::U64 enemyPieces = isWhite ? _blackPiecesBitmask : _whitePiecesBitmask;
            bits::U64 capturableKnightMoves = knightBitMask & enemyPieces;

            bits::getBitIndices(_freeMovesIndices, freeKnightMoves);
            bits::getBitIndices(_capturableMovesIndices, capturableKnightMoves);

            for (int freeKnightMoveIndex : _freeMovesIndices) {
                addMove(currentKnightIndex, freeKnightMoveIndex, currentPieceType);
            }

            for (int capturableKnightMoveIndex : _capturableMovesIndices) {
                addMove(currentKnightIndex, capturableKnightMoveIndex, currentPieceType);
            }
        }
    }

    void MoveGenerator::genQueenMoves(bool isWhite) {
        PieceType currentPieceType = isWhite ? PieceType::W_QUEEN : PieceType::B_QUEEN;
        bits::getBitIndices(_queenIndices, _board.getBitboard(currentPieceType));

        for (int currentQueenIndex : _queenIndices) {
            bits::StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
            bits::DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
            int queenRank = bits::rankFromBitIndex(currentQueenIndex);
            int queenFile = bits::fileFromBitIndex(currentQueenIndex);

            getMovesFromStraightRay(straightRays.north, true, false, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);
            getMovesFromStraightRay(straightRays.east, false, true, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);
            getMovesFromStraightRay(straightRays.south, false, false, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);
            getMovesFromStraightRay(straightRays.west, true, true, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);

            getMovesFromDiagonalRay(diagonalRays.northEast, true, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);
            getMovesFromDiagonalRay(diagonalRays.southEast, false, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);
            getMovesFromDiagonalRay(diagonalRays.southWest, false, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);
            getMovesFromDiagonalRay(diagonalRays.northWest, true, isWhite, currentQueenIndex, currentPieceType, queenRank, queenFile);
        }
    }

    void MoveGenerator::genKingMoves(bool isWhite) {
        PieceType currentPieceType = isWhite ? PieceType::W_KING : PieceType::B_KING;
        bits::getBitIndices(_kingIndices, _board.getBitboard(currentPieceType));

        for (int currentKingIndex : _kingIndices) {
            bits::U64 kingBitMask = _kingBitmasks[currentKingIndex];

            bits::U64 freeKingMoves = kingBitMask & _emptySquaresBitmask;
            bits::U64 enemyPieces = isWhite ? _blackPiecesBitmask : _whitePiecesBitmask;
            bits::U64 capturableKingMoves = kingBitMask & enemyPieces;

            bits::getBitIndices(_freeMovesIndices, freeKingMoves);
            bits::getBitIndices(_capturableMovesIndices, capturableKingMoves);

            for (int freeKingMoveIndex : _freeMovesIndices) {
                addMove(currentKingIndex, freeKingMoveIndex, currentPieceType);
            }

            for (int capturableKingMoveIndex : _capturableMovesIndices) {
                addMove(currentKingIndex, capturableKingMoveIndex, currentPieceType);
            }
        }
    }

    void MoveGenerator::genPawnMoves(bool isWhite) {
        std::vector<bits::U64> straightPawnMoveBitmasks;
        std::vector<bits::U64> capturePawnMoveBitmasks;

        if (isWhite) {
            straightPawnMoveBitmasks = _whitePawnStraightMoveBitmasks;
            capturePawnMoveBitmasks = _whitePawnCaptureMoveBitmasks;
        } else {
            straightPawnMoveBitmasks = _blackPawnStraightMoveBitmasks;
            capturePawnMoveBitmasks = _blackPawnCaptureMoveBitmasks;
        }

        PieceType currentPieceType = isWhite ? PieceType::W_PAWN : PieceType::B_PAWN;
        bits::getBitIndices(_pawnIndices, _board.getBitboard(currentPieceType));

        for (int currentPawnIndex : _pawnIndices) {
            bits::U64 straightPawnMoveBitmask = straightPawnMoveBitmasks[currentPawnIndex];
            bits::U64 capturePawnMoveBitmask = capturePawnMoveBitmasks[currentPawnIndex];

            bits::U64 freePawnMoves = straightPawnMoveBitmask & _emptySquaresBitmask;
            bits::U64 enemyPieces = isWhite ? _blackPiecesBitmask : _whitePiecesBitmask;
            bits::U64 enPessantTarget = _board.getEnPessantTarget();
            bits::U64 capturablePawnMoves = capturePawnMoveBitmask & (enemyPieces | enPessantTarget);

            bits::getBitIndices(_freeMovesIndices, freePawnMoves);
            bits::getBitIndices(_capturableMovesIndices, capturablePawnMoves);
            int offset = isWhite ? 8 : -8;

            if (_freeMovesIndices.size() == 2) {
                addMove(currentPawnIndex, _freeMovesIndices[0], currentPieceType);
                addMove(currentPawnIndex, _freeMovesIndices[1], currentPieceType);
            } else if (_freeMovesIndices.size() == 1 && _freeMovesIndices[0] == currentPawnIndex + offset) {
                // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
                addMove(currentPawnIndex, _freeMovesIndices[0], currentPieceType);
            }

            for (int capturablePawnMoveIndex : _capturableMovesIndices) {
                addMove(currentPawnIndex, capturablePawnMoveIndex, currentPieceType);
            }
        }   
    }
}