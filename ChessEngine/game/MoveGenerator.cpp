#include "MoveGenerator.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/bits/KnightBitMasks.h"
#include "ChessEngine/bits/KingBitMasks.h"
#include "ChessEngine/bits/PawnBitMasks.h"
#include "ChessEngine/bits/BitBasics.h"
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
    MoveGenerator::MoveGenerator(ChessBoard& board) : _board(board) {
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

        _whiteKingSideCastleBitmask = bits::whiteKingSideCastleMask;
        _whiteQueenSideCastleBitmask = bits::whiteQueenSideCastleMask;
        _blackKingSideCastleBitmask = bits::blackKingSideCastleMask;
        _blackQueenSideCastleBitmask = bits::blackQueenSideCastleMask;

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

    void MoveGenerator::addMove(int bitIndexFrom, int bitIndexTo, int flag) {
        _moves[_moveIndex] = Move(bitIndexFrom, bitIndexTo, flag);
        _moveIndex++;
    }

    // count the number of elements in a vector that are not null
    int MoveGenerator::getNumLegalMoves() {
        int numLegalMoves = 0;

        for (Move move : _moves) {
            if (move.getMove() != 0) {
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
        genCastlingMoves(isWhite);

        return _moves;
    }

    void MoveGenerator::resetMoves() {
        _moves.clear();
        _moveIndex = 0;
    }

    void MoveGenerator::addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom) {
        bits::getBitIndices(_freeRayIndices, freeRay);

        for (int bitIndex : _freeRayIndices) {
            addMove(bitIndexFrom, bitIndex, Move::QUITE_FLAG);
        }
    }

    void MoveGenerator::addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom) {
        bool blockerIsWhite = bits::getBit(_whitePiecesBitmask, blockerIndex);

        if (blockerIsWhite != isWhite) {
            addMove(bitIndexFrom, blockerIndex, Move::CAPTURE_FLAG);
        }
    }

    void MoveGenerator::addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex, bool alongFile, 
                                                    bool startFromBlocker, int rookRank, 
                                                    int rookFile, 
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
            addMove(bitIndexFrom, rankOrFileIndex, Move::QUITE_FLAG);
        }
    }

    void MoveGenerator::addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, 
                                                    int bishopRank, int bishopFile, 
                                                    int bitIndexFrom) {
        
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
            addMove(bitIndexFrom, rankOrFileIndex, Move::QUITE_FLAG);
        }
    }

    void MoveGenerator::getMovesFromStraightRay(bits::U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, int pieceRank, int pieceFile) {
            bits::U64 blockerBitMask = ray & _occupiedBitmask;          

            if (blockerBitMask != 0) {
                int blockerIndex = blockerOnLSB ? bits::indexOfLSB(blockerBitMask) : bits::indexOfMSB(blockerBitMask);
                addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex);
                addMovesBetweenBlockerAndPieceOnStraightRay(blockerIndex, alongFile, blockerOnLSB, pieceRank, pieceFile, pieceIndex);
            } else {
                addMovesFromFreeRay(ray, pieceIndex);
            }
    }

    void MoveGenerator::getMovesFromDiagonalRay(bits::U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, int pieceRank, int pieceFile) {
        bits::U64 blockerBitMask = ray & _occupiedBitmask;

        if (blockerBitMask != 0) {
            int blockerIndex = blockerOnLSB ? bits::indexOfLSB(blockerBitMask) : bits::indexOfMSB(blockerBitMask);
            addMoveIfBlockerIsEnemy(blockerIndex, isWhite, pieceIndex);
            addMovesBetweenBlockerAndPieceOnDiagonalRay(blockerIndex, blockerOnLSB, pieceRank, pieceFile, pieceIndex);
        } else {
            addMovesFromFreeRay(ray, pieceIndex);
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

            getMovesFromStraightRay(rays.north, true, false, isWhite, currentRookIndex, rookRank, rookFile);
            getMovesFromStraightRay(rays.east, false, true, isWhite, currentRookIndex, rookRank, rookFile);
            getMovesFromStraightRay(rays.south, false, false, isWhite, currentRookIndex, rookRank, rookFile);
            getMovesFromStraightRay(rays.west, true, true, isWhite, currentRookIndex, rookRank, rookFile);
        }
    }

    void MoveGenerator::genBishopMoves(bool isWhite) {
        PieceType currentPieceType = isWhite ? PieceType::W_BISHOP : PieceType::B_BISHOP;
        bits::getBitIndices(_bishopIndices, _board.getBitboard(currentPieceType));

        for (int currentBishopIndex : _bishopIndices) {
            bits::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
            int bishopRank = bits::rankFromBitIndex(currentBishopIndex);
            int bishopFile = bits::fileFromBitIndex(currentBishopIndex);

            getMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile);
            getMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile);
            getMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile);
            getMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile);
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
                addMove(currentKnightIndex, freeKnightMoveIndex, Move::QUITE_FLAG);
            }

            for (int capturableKnightMoveIndex : _capturableMovesIndices) {
                addMove(currentKnightIndex, capturableKnightMoveIndex, Move::CAPTURE_FLAG);
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

            getMovesFromStraightRay(straightRays.north, true, false, isWhite, currentQueenIndex, queenRank, queenFile);
            getMovesFromStraightRay(straightRays.east, false, true, isWhite, currentQueenIndex, queenRank, queenFile);
            getMovesFromStraightRay(straightRays.south, false, false, isWhite, currentQueenIndex, queenRank, queenFile);
            getMovesFromStraightRay(straightRays.west, true, true, isWhite, currentQueenIndex, queenRank, queenFile);

            getMovesFromDiagonalRay(diagonalRays.northEast, true, isWhite, currentQueenIndex, queenRank, queenFile);
            getMovesFromDiagonalRay(diagonalRays.southEast, false, isWhite, currentQueenIndex, queenRank, queenFile);
            getMovesFromDiagonalRay(diagonalRays.southWest, false, isWhite, currentQueenIndex, queenRank, queenFile);
            getMovesFromDiagonalRay(diagonalRays.northWest, true, isWhite, currentQueenIndex, queenRank, queenFile);
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
                addMove(currentKingIndex, freeKingMoveIndex, Move::QUITE_FLAG);
            }

            for (int capturableKingMoveIndex : _capturableMovesIndices) {
                addMove(currentKingIndex, capturableKingMoveIndex, Move::CAPTURE_FLAG);
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
            bits::U64 capturablePawnMoves = capturePawnMoveBitmask & enemyPieces;

            bits::getBitIndices(_freeMovesIndices, freePawnMoves);
            bits::getBitIndices(_capturableMovesIndices, capturablePawnMoves);
            int offset = isWhite ? 8 : -8;
            bool canPromote = (isWhite && bits::rankFromBitIndex(currentPawnIndex) == 6) || (!isWhite && bits::rankFromBitIndex(currentPawnIndex) == 1);

            if (_freeMovesIndices.size() == 2) {
                int singleStepIndex = (isWhite ? 0 : 1);
                int doubleStepIndex = (isWhite ? 1 : 0);

                addMove(currentPawnIndex, _freeMovesIndices[singleStepIndex], (Move::QUITE_FLAG));
                addMove(currentPawnIndex, _freeMovesIndices[doubleStepIndex], (Move::DOUBLE_PAWN_PUSH_FLAG));
            } else if (_freeMovesIndices.size() == 1 && _freeMovesIndices[0] == currentPawnIndex + offset) {
                // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
                if (canPromote) {
                    addMove(currentPawnIndex, _freeMovesIndices[0], Move::KNIGHT_PROMO_FLAG);
                    addMove(currentPawnIndex, _freeMovesIndices[0], Move::BISHOP_PROMO_FLAG);
                    addMove(currentPawnIndex, _freeMovesIndices[0], Move::ROOK_PROMO_FLAG);
                    addMove(currentPawnIndex, _freeMovesIndices[0], Move::QUEEN_PROMO_FLAG);
                } else {
                    addMove(currentPawnIndex, _freeMovesIndices[0], Move::QUITE_FLAG);
                }
            }

            for (int capturablePawnMoveIndex : _capturableMovesIndices) {
                if (canPromote) {
                    addMove(currentPawnIndex, capturablePawnMoveIndex, Move::KNIGHT_PROMO_CAPTURE_FLAG);
                    addMove(currentPawnIndex, capturablePawnMoveIndex, Move::BISHOP_PROMO_CAPTURE_FLAG);
                    addMove(currentPawnIndex, capturablePawnMoveIndex, Move::ROOK_PROMO_CAPTURE_FLAG);
                    addMove(currentPawnIndex, capturablePawnMoveIndex, Move::QUEEN_PROMO_CAPTURE_FLAG);
                } else {
                    addMove(currentPawnIndex, capturablePawnMoveIndex, Move::CAPTURE_FLAG);
                }
            }

            if ((capturePawnMoveBitmask & enPessantTarget) != 0) {
                addMove(currentPawnIndex, bits::indexOfLSB(capturePawnMoveBitmask & enPessantTarget), Move::EP_CAPTURE_FLAG);
            }
        }
    }

    void MoveGenerator::genCastlingMoves(bool isWhite) {
        if (isWhite && !_board.getHasCastled(true)) {
            // King side castling
            if (!_board.kingSideCastlersHasMoved(true)) {
                if ((_whiteKingSideCastleBitmask & _occupiedBitmask) == 0) {
                    addMove(0, 0, Move::KING_CASTLE_FLAG);
                }
            }

            // Queen side castling
            if (!_board.queenSideCastlersHasMoved(true)) {
                if ((_whiteQueenSideCastleBitmask & _occupiedBitmask) == 0) {
                    addMove(0, 0, Move::QUEEN_CASTLE_FLAG);
                }
            }

        } else if (!isWhite && !_board.getHasCastled(false)) {
            // King side castling
            if (!_board.kingSideCastlersHasMoved(false)) {
                if ((_blackKingSideCastleBitmask & _occupiedBitmask) == 0) {
                    addMove(0, 0, Move::KING_CASTLE_FLAG);
                }
            }

            // Queen side castling
            if (!_board.queenSideCastlersHasMoved(false)) {
                if ((_blackQueenSideCastleBitmask & _occupiedBitmask) == 0) {
                    addMove(0, 0, Move::QUEEN_CASTLE_FLAG);
                }
            }
        }
    }

    bool MoveGenerator::checkStraightRay(bits::U64& straightRay, bool firstBlockerOnLSB, bits::U64& opponentRooksAndQueens) {
        bits::U64 rooksAndQueensBlockerBitMask = straightRay & opponentRooksAndQueens;
        
        // There must be a rook or a queen on the file or rank to be in check
        if ((rooksAndQueensBlockerBitMask) != 0) {
            bits::U64 occupiedBlockerBitMask = straightRay & _occupiedBitmask;

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

    bool MoveGenerator::checkDiagonalRay(bits::U64& diagonalRay, bool firstBlockerOnLSB, bits::U64& opponentBishopsAndQueens) {
        bits::U64 bishopsAndQueensBlockerBitMask = diagonalRay & opponentBishopsAndQueens;

        if ((bishopsAndQueensBlockerBitMask) != 0) {
            bits::U64 occupiedBlockerBitMask = diagonalRay & _occupiedBitmask;

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

    bool MoveGenerator::isInCheck(bool isWhite) {
        int kingIndex = bits::indexOfLSB(isWhite ? _board.getBitboard(PieceType::W_KING)
                                                 : _board.getBitboard(PieceType::B_KING));

        bits::StraightRays straightRays = _straightRayBitmasks[kingIndex];
        bits::DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];
        bits::U64 knightMoves = _knightBitmasks[kingIndex];
        bits::U64 pawnAttackingMoves = isWhite ? _whitePawnCaptureMoveBitmasks[kingIndex] 
                                               : _blackPawnCaptureMoveBitmasks[kingIndex];
        bits::U64 opponentRooksAndQueens = isWhite ? _board.getBitboard(PieceType::B_ROOK) | _board.getBitboard(PieceType::B_QUEEN) 
                                                   : _board.getBitboard(PieceType::W_ROOK) | _board.getBitboard(PieceType::W_QUEEN);

        bits::U64 opponentBishopsAndQueens = isWhite ? _board.getBitboard(PieceType::B_BISHOP) | _board.getBitboard(PieceType::B_QUEEN) 
                                                     : _board.getBitboard(PieceType::W_BISHOP) | _board.getBitboard(PieceType::W_QUEEN);

        if ((pawnAttackingMoves & _board.getBitboard(isWhite ? PieceType::B_PAWN : PieceType::W_PAWN)) != 0) {
            return true;
        }

        if ((knightMoves & _board.getBitboard(isWhite ? PieceType::B_KNIGHT : PieceType::W_KNIGHT)) != 0) {
            return true;
        }

        if (checkStraightRay(straightRays.north, true, opponentRooksAndQueens)) {
            return true;
        }

        if (checkStraightRay(straightRays.east, false, opponentRooksAndQueens)) {
            return true;
        }

        if (checkStraightRay(straightRays.south, false, opponentRooksAndQueens)) {
            return true;
        }

        if (checkStraightRay(straightRays.west, true, opponentRooksAndQueens)) {
            return true;
        }

        if (checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens)) {
            return true;
        }

        if (checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens)) {
            return true;
        }

        if (checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens)) {
            return true;
        }

        if (checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens)) {
            return true;
        }

        return false;
    }
}