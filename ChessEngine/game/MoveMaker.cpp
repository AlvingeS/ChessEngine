#include "MoveMaker.h"

namespace game {

    MoveMaker::MoveMaker(
        BitBoards& bitboards,
        SquaresLookup& squaresLookup,
        GameStateBitMasks& gameStateBitmasks,
        perft::SearchMemory& searchMemory,
        ZHasher& zHasher
    ) : _bitboards(bitboards), 
        _squaresLookup(squaresLookup), 
        _gameStateBitMasks(gameStateBitmasks), 
        _searchMemory(searchMemory), 
        _zHasher(zHasher)
    {}

    void MoveMaker::makeCastleMove(bool isWhite, bool isKingSide) {
        if (isWhite) {
            if (isKingSide) {
                _bitboards.getWhiteKingBitboard() &= ~(1ULL << 3);
                _bitboards.getWhiteKingBitboard() |= (1ULL << 1);
                _bitboards.getWhiteRooksBitboard() &= ~(1ULL);
                _bitboards.getWhiteRooksBitboard() |= (1ULL << 2);
                _squaresLookup.setPieceTypeAtIndex(3, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(1, PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(0, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(2, PieceType::W_ROOK);
            } else {
                _bitboards.getWhiteKingBitboard() &= ~(1ULL << 3);
                _bitboards.getWhiteKingBitboard() |= (1ULL << 5);
                _bitboards.getWhiteRooksBitboard() &= ~(1ULL << 7);
                _bitboards.getWhiteRooksBitboard() |= (1ULL << 4);
                _squaresLookup.setPieceTypeAtIndex(3, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(5, PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(7, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(4, PieceType::W_ROOK);
            }
        } else {
            if (isKingSide) {
                _bitboards.getBlackKingBitboard() &= ~(1ULL << 59);
                _bitboards.getBlackKingBitboard() |= (1ULL << 57);
                _bitboards.getBlackRooksBitboard() &= ~(1ULL << 56);
                _bitboards.getBlackRooksBitboard() |= (1ULL << 58);
                _squaresLookup.setPieceTypeAtIndex(59, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(57, PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(56, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(58, PieceType::B_ROOK);
            } else {
                _bitboards.getBlackKingBitboard() &= ~(1ULL << 59);
                _bitboards.getBlackKingBitboard() |= (1ULL << 61);
                _bitboards.getBlackRooksBitboard() &= ~(1ULL << 63);
                _bitboards.getBlackRooksBitboard() |= (1ULL << 60);
                _squaresLookup.setPieceTypeAtIndex(59, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(61, PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(63, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(60, PieceType::B_ROOK);
            }
        }
    }

    void MoveMaker::unmakeCastleMove(bool wasWhite, bool wasKingSide) {
        if (wasWhite) {
            if (wasKingSide) {
                _bitboards.getWhiteKingBitboard() &= ~(1ULL << 1);
                _bitboards.getWhiteKingBitboard() |= (1ULL << 3);
                _bitboards.getWhiteRooksBitboard() &= ~(1ULL << 2);
                _bitboards.getWhiteRooksBitboard() |= (1ULL);
                _squaresLookup.setPieceTypeAtIndex(1, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(3, PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(2, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(0, PieceType::W_ROOK);
            } else {
                _bitboards.getWhiteKingBitboard() &= ~(1ULL << 5);
                _bitboards.getWhiteKingBitboard() |= (1ULL << 3);
                _bitboards.getWhiteRooksBitboard() &= ~(1ULL << 4);
                _bitboards.getWhiteRooksBitboard() |= (1ULL << 7);
                _squaresLookup.setPieceTypeAtIndex(5, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(3, PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(4, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(7, PieceType::W_ROOK);
            }
        } else {
            if (wasKingSide) {
                _bitboards.getBlackKingBitboard() &= ~(1ULL << 57);
                _bitboards.getBlackKingBitboard() |= (1ULL << 59);
                _bitboards.getBlackRooksBitboard() &= ~(1ULL << 58);
                _bitboards.getBlackRooksBitboard() |= (1ULL << 56);
                _squaresLookup.setPieceTypeAtIndex(57, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(59, PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(58, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(56, PieceType::B_ROOK);
            } else {
                _bitboards.getBlackKingBitboard() &= ~(1ULL << 61);
                _bitboards.getBlackKingBitboard() |= (1ULL << 59);
                _bitboards.getBlackRooksBitboard() &= ~(1ULL << 60);
                _bitboards.getBlackRooksBitboard() |= (1ULL << 63);
                _squaresLookup.setPieceTypeAtIndex(61, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(59, PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(60, PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(63, PieceType::B_ROOK);
            }
        }
    }

    PieceType MoveMaker::getPromotionPieceType(int promotionFlag, bool isWhite) {
        switch(promotionFlag) {
            case Move::KNIGHT_PROMO_FLAG:
                return isWhite ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
                break;
            case Move::BISHOP_PROMO_FLAG:
                return isWhite ? PieceType::W_BISHOP : PieceType::B_BISHOP;
                break;
            case Move::ROOK_PROMO_FLAG:
                return isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
                break;
            case Move::QUEEN_PROMO_FLAG:
                return isWhite ? PieceType::W_QUEEN : PieceType::B_QUEEN;
                break;
            case Move::KNIGHT_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
                break;
            case Move::BISHOP_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_BISHOP : PieceType::B_BISHOP;
                break;
            case Move::ROOK_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
                break;
            case Move::QUEEN_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_QUEEN : PieceType::B_QUEEN;
                break;
        }

        return PieceType::EMPTY;
    }

    void MoveMaker::makeMove(Move move, bool isWhite, int currentDepth) {
        // If the move is a castle, update the bitboards and return
        if (move.isAnyCastle()) { [[unlikely]]
            makeCastleMove(isWhite, move.isKingCastle());

            // Update white or black bitmasks
            if (isWhite) {
                if (move.isKingCastle()) {
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 1);
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 2);
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 3);
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 0);
                } else {
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 5);
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 4);
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 3);
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 7);
                }
            } else {
                if (move.isKingCastle()) {
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 57);
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 58);
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 59);
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 56);
                } else {
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 61);
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 60);
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 59);
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 63);
                }
            }

            _gameStateBitMasks.fillOccupiedPiecesBitmask();
            _gameStateBitMasks.fillEmptySquaresBitmask();

            return;
        }

        // Get the from and to indices
        int from = move.getBitIndexFrom();
        int to = move.getBitIndexTo();
        
        // Piece type of piece being moved
        PieceType movedPieceType = _squaresLookup.getPieceTypeAtIndex(from);
        _bitboards.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) &= ~(1ULL << from);
        
        if (isWhite) {
            _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << from);
        } else {
            _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << from);
        }

        _squaresLookup.setPieceTypeAtIndex(from, PieceType::EMPTY);

        // FIXME: THIS IS TEMPORARY
        // if (!move.isAnyCapture() && (movedPieceType != PieceType::W_PAWN && movedPieceType != PieceType::B_PAWN)) {
        //     _board.getNoCaptureOrPawnMoveCount() += 0;
        // } else {
        //     _board.getNoCaptureOrPawnMoveCount() = 0;
        // }

        // If the move is a capture, update the last captured piece and its bitboard
        if (move.isAnyCapture()) { 
            int captureIndex = move.isEpCapture() ? (isWhite ? to - 8 : to + 8) : to;
            PieceType capturedPieceType = _squaresLookup.getPieceTypeAtIndex(captureIndex);
            _searchMemory.setLastCapturedPieceAtDepth(currentDepth, capturedPieceType);

            _bitboards.getBitboardFromIndex(pieceTypeToInt(capturedPieceType)) &= ~(1ULL << captureIndex);

            if (isWhite) {
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << captureIndex);
            } else {
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << captureIndex);
            }

            if (move.isEpCapture()) { [[unlikely]]
                _squaresLookup.setPieceTypeAtIndex(captureIndex, PieceType::EMPTY);
            }
        }

        if (move.isAnyPromo()) { [[unlikely]]
            PieceType promotionPieceType = getPromotionPieceType(move.getFlag(), isWhite);
            _bitboards.getBitboardFromIndex(pieceTypeToInt(promotionPieceType)) |= (1ULL << to);
            _squaresLookup.setPieceTypeAtIndex(to, promotionPieceType);
        } else {
            _bitboards.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) |= (1ULL << to);
            _squaresLookup.setPieceTypeAtIndex(to, movedPieceType);
        }

        if (isWhite) {
            _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << to);
        } else {
            _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << to);
        }

        if (move.isDoublePawnPush()) { [[unlikely]]
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, isWhite ? (1ULL << (to - 8)) : (1ULL << (to + 8)));
        } else {
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        }

        if (not move.isAnyCapture() && (movedPieceType != PieceType::W_PAWN && movedPieceType != PieceType::B_PAWN)) {
            _searchMemory.incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        } else {
            _searchMemory.resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        }

        _gameStateBitMasks.fillOccupiedPiecesBitmask();
        _gameStateBitMasks.fillEmptySquaresBitmask();
    }

    void MoveMaker::makeTemporaryKingMove(bool isWhite, bool isKingSide) {
        U64 kingBitboard = isWhite ? _bitboards.getWhiteKingBitboard() : _bitboards.getBlackKingBitboard();
        int from = isWhite ? 3 : 59;
        int to = isKingSide ? (isWhite ? 2 : 58) : (isWhite ? 4 : 60);

        kingBitboard &= ~(1ULL << from);
        kingBitboard |= (1ULL << to);

        if (isWhite) {
            _bitboards.getWhiteKingBitboard() = kingBitboard;
        } else {
            _bitboards.getBlackKingBitboard() = kingBitboard;
        }
    }

    void MoveMaker::unmakeTemporaryKingMove(bool isWhite, bool isKingSide) {
        U64 kingBitboard = isWhite ? _bitboards.getWhiteKingBitboard() : _bitboards.getBlackKingBitboard();
        int from = isKingSide ? (isWhite ? 2 : 58) : (isWhite ? 4 : 60);
        int to = isWhite ? 3 : 59;

        kingBitboard &= ~(1ULL << from);
        kingBitboard |= (1ULL << to);

        if (isWhite) {
            _bitboards.getWhiteKingBitboard() = kingBitboard;
        } else {
            _bitboards.getBlackKingBitboard() = kingBitboard;
        }
    }

    void MoveMaker::unmakeMove(Move move, bool wasWhite, int currentDepth) {
        // If the move is a castle, update the bitboards and return
        if (move.isAnyCastle()) {
            unmakeCastleMove(wasWhite, move.isKingCastle());

            // Update white or black bitmasks
            if (wasWhite) {
                if (move.isKingCastle()) {
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 1);
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 2);
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 3);
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 0);
                } else {
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 5);
                    _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 4);
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 3);
                    _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 7);
                }
            } else {
                if (move.isKingCastle()) {
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 57);
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 58);
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 59);
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 56);
                } else {
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 61);
                    _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 60);
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 59);
                    _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 63);
                }
            }

            _gameStateBitMasks.fillOccupiedPiecesBitmask();
            _gameStateBitMasks.fillEmptySquaresBitmask();

            return;
        }

        // Get the from and to indices
        int from = move.getBitIndexFrom();
        int to = move.getBitIndexTo();
        
        // Piece type of piece being moved
        PieceType movedPieceType;

        // If the move was a promotion, set the moved piece to a pawn of the same color
        // Else, set the moved piece to the piece occupying the to square
        bool moveIsAnyPromo = move.isAnyPromo();
        
        if (moveIsAnyPromo) {
            movedPieceType = wasWhite ? PieceType::W_PAWN : PieceType::B_PAWN;
        } else {
            movedPieceType = _squaresLookup.getPieceTypeAtIndex(to);
        }

        // Place back the moved piece
        _bitboards.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) |= (1ULL << from);
        _squaresLookup.setPieceTypeAtIndex(from, movedPieceType);

        if (wasWhite) {
            _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << from);
        } else {
            _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << from);
        }

        // If the move was not a promotion, remove the piece in the bitboard
        // Else, remove the bit for the promoted piece
        if (not moveIsAnyPromo) {
            _bitboards.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) &= ~(1ULL << to);
        } else {
            PieceType promotionPieceType = getPromotionPieceType(move.getFlag(), wasWhite);
            _bitboards.getBitboardFromIndex(pieceTypeToInt(promotionPieceType)) &= ~(1ULL << to);
        }

        if (wasWhite) {
            _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << to);
        } else {
            _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << to);
        }

        // If the move was a capture, place back the captured piece
        // else set the square to empty
        if (move.isAnyCapture()) {
            int captureIndex = move.isEpCapture() ? (wasWhite ? to - 8 : to + 8) : to;
            game::PieceType capturedPieceType = _searchMemory.getLastCapturedPieceAtDepth(currentDepth);
            _bitboards.getBitboardFromIndex(pieceTypeToInt(capturedPieceType)) |= (1ULL << captureIndex);
            _squaresLookup.setPieceTypeAtIndex(captureIndex, capturedPieceType);

            if (move.isEpCapture()) {
                _squaresLookup.setPieceTypeAtIndex(to, PieceType::EMPTY);
            }

            if (wasWhite) {
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << captureIndex);
            } else {
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << captureIndex);
            }

        } else {
            _squaresLookup.setPieceTypeAtIndex(to, PieceType::EMPTY);
        }

        if (move.isDoublePawnPush()) {
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        }

        if (not move.isAnyCapture() && (movedPieceType != PieceType::W_PAWN && movedPieceType != PieceType::B_PAWN)) {
            _searchMemory.decrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        }

        _gameStateBitMasks.fillOccupiedPiecesBitmask();
        _gameStateBitMasks.fillEmptySquaresBitmask();
    }
}