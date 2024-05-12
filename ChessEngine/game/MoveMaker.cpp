#include "MoveMaker.h"

namespace game {

    MoveMaker::MoveMaker(ChessBoard& board) : _board(board) {}

    void MoveMaker::makeCastleMove(bool isWhite, bool isKingSide) {
        if (isWhite) {
            if (isKingSide) {
                _board.getWhiteKingBitboard() &= ~(1ULL << 3);
                _board.getWhiteKingBitboard() |= (1ULL << 1);
                _board.getWhiteRooksBitboard() &= ~(1ULL);
                _board.getWhiteRooksBitboard() |= (1ULL << 2);
                _board.setPieceTypeAtIndex(3, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(1, PieceType::W_KING);
                _board.setPieceTypeAtIndex(0, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(2, PieceType::W_ROOK);
            } else {
                _board.getWhiteKingBitboard() &= ~(1ULL << 3);
                _board.getWhiteKingBitboard() |= (1ULL << 5);
                _board.getWhiteRooksBitboard() &= ~(1ULL << 7);
                _board.getWhiteRooksBitboard() |= (1ULL << 4);
                _board.setPieceTypeAtIndex(3, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(5, PieceType::W_KING);
                _board.setPieceTypeAtIndex(7, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(4, PieceType::W_ROOK);
            }
        } else {
            if (isKingSide) {
                _board.getBlackKingBitboard() &= ~(1ULL << 59);
                _board.getBlackKingBitboard() |= (1ULL << 57);
                _board.getBlackRooksBitboard() &= ~(1ULL << 56);
                _board.getBlackRooksBitboard() |= (1ULL << 58);
                _board.setPieceTypeAtIndex(59, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(57, PieceType::B_KING);
                _board.setPieceTypeAtIndex(56, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(58, PieceType::B_ROOK);
            } else {
                _board.getBlackKingBitboard() &= ~(1ULL << 59);
                _board.getBlackKingBitboard() |= (1ULL << 61);
                _board.getBlackRooksBitboard() &= ~(1ULL << 63);
                _board.getBlackRooksBitboard() |= (1ULL << 60);
                _board.setPieceTypeAtIndex(59, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(61, PieceType::B_KING);
                _board.setPieceTypeAtIndex(63, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(60, PieceType::B_ROOK);
            }
        }
    }

    void MoveMaker::unmakeCastleMove(bool wasWhite, bool wasKingSide) {
        if (wasWhite) {
            if (wasKingSide) {
                _board.getWhiteKingBitboard() &= ~(1ULL << 1);
                _board.getWhiteKingBitboard() |= (1ULL << 3);
                _board.getWhiteRooksBitboard() &= ~(1ULL << 2);
                _board.getWhiteRooksBitboard() |= (1ULL);
                _board.setPieceTypeAtIndex(1, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(3, PieceType::W_KING);
                _board.setPieceTypeAtIndex(2, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(0, PieceType::W_ROOK);
            } else {
                _board.getWhiteKingBitboard() &= ~(1ULL << 5);
                _board.getWhiteKingBitboard() |= (1ULL << 3);
                _board.getWhiteRooksBitboard() &= ~(1ULL << 4);
                _board.getWhiteRooksBitboard() |= (1ULL << 7);
                _board.setPieceTypeAtIndex(5, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(3, PieceType::W_KING);
                _board.setPieceTypeAtIndex(4, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(7, PieceType::W_ROOK);
            }
        } else {
            if (wasKingSide) {
                _board.getBlackKingBitboard() &= ~(1ULL << 57);
                _board.getBlackKingBitboard() |= (1ULL << 59);
                _board.getBlackRooksBitboard() &= ~(1ULL << 58);
                _board.getBlackRooksBitboard() |= (1ULL << 56);
                _board.setPieceTypeAtIndex(57, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(59, PieceType::B_KING);
                _board.setPieceTypeAtIndex(58, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(56, PieceType::B_ROOK);
            } else {
                _board.getBlackKingBitboard() &= ~(1ULL << 61);
                _board.getBlackKingBitboard() |= (1ULL << 59);
                _board.getBlackRooksBitboard() &= ~(1ULL << 60);
                _board.getBlackRooksBitboard() |= (1ULL << 63);
                _board.setPieceTypeAtIndex(61, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(59, PieceType::B_KING);
                _board.setPieceTypeAtIndex(60, PieceType::EMPTY);
                _board.setPieceTypeAtIndex(63, PieceType::B_ROOK);
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

    void MoveMaker::makeMove(Move move, bool isWhite) {
        // If the move is a castle, update the bitboards and return
        if (move.isAnyCastle()) {
            makeCastleMove(isWhite, move.isKingCastle());

            // Update white or black bitmasks
            if (isWhite) {
                if (move.isKingCastle()) {
                    _board.getWhitePiecesBitmask() |= (1ULL << 1);
                    _board.getWhitePiecesBitmask() |= (1ULL << 2);
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 3);
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 0);
                } else {
                    _board.getWhitePiecesBitmask() |= (1ULL << 5);
                    _board.getWhitePiecesBitmask() |= (1ULL << 4);
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 3);
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 7);
                }
            } else {
                if (move.isKingCastle()) {
                    _board.getBlackPiecesBitmask() |= (1ULL << 57);
                    _board.getBlackPiecesBitmask() |= (1ULL << 58);
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 59);
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 56);
                } else {
                    _board.getBlackPiecesBitmask() |= (1ULL << 61);
                    _board.getBlackPiecesBitmask() |= (1ULL << 60);
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 59);
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 63);
                }
            }

            _board.fillOccupiedPiecesBitmask();
            _board.fillEmptySquaresBitmask();

            return;
        }

        // Get the from and to indices
        int from = move.getBitIndexFrom();
        int to = move.getBitIndexTo();
        
        // Piece type of piece being moved
        PieceType movedPieceType = _board.getPieceTypeAtIndex(from);
        _board.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) &= ~(1ULL << from);
        
        if (isWhite) {
            _board.getWhitePiecesBitmask() &= ~(1ULL << from);
        } else {
            _board.getBlackPiecesBitmask() &= ~(1ULL << from);
        }

        _board.setPieceTypeAtIndex(from, PieceType::EMPTY);

        // FIXME: THIS IS TEMPORARY
        if (!move.isAnyCapture() && (movedPieceType != PieceType::W_PAWN && movedPieceType != PieceType::B_PAWN)) {
            _board.getNoCaptureOrPawnMoveCount() += 0;
        } else {
            _board.getNoCaptureOrPawnMoveCount() = 0;
        }

        // If the move is a capture, update the last captured piece and its bitboard
        if (move.isAnyCapture()) {
            int captureIndex = move.isEpCapture() ? (isWhite ? to - 8 : to + 8) : to;
            PieceType capturedPieceType = _board.getPieceTypeAtIndex(captureIndex);
            _board.setLastCapturedPiece(capturedPieceType);

            _board.getBitboardFromIndex(pieceTypeToInt(capturedPieceType)) &= ~(1ULL << captureIndex);

            if (isWhite) {
                _board.getBlackPiecesBitmask() &= ~(1ULL << captureIndex);
            } else {
                _board.getWhitePiecesBitmask() &= ~(1ULL << captureIndex);
            }

            if (move.isEpCapture()) {
                _board.setPieceTypeAtIndex(captureIndex, PieceType::EMPTY);
            }
        }

        if (move.isAnyPromo()) {
            PieceType promotionPieceType = getPromotionPieceType(move.getFlag(), isWhite);
            _board.getBitboardFromIndex(pieceTypeToInt(promotionPieceType)) |= (1ULL << to);
            _board.setPieceTypeAtIndex(to, promotionPieceType);
        } else {
            _board.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) |= (1ULL << to);
            _board.setPieceTypeAtIndex(to, movedPieceType);
        }

        if (isWhite) {
            _board.getWhitePiecesBitmask() |= (1ULL << to);
        } else {
            _board.getBlackPiecesBitmask() |= (1ULL << to);
        }

        if (move.isDoublePawnPush()) {
            _board.setEnPessantTarget(isWhite ? (1ULL << (to - 8)) : (1ULL << (to + 8)));
        } else {
            _board.setEnPessantTarget(0ULL);
        }

        _board.fillOccupiedPiecesBitmask();
        _board.fillEmptySquaresBitmask();
    }

    void MoveMaker::makeTemporaryKingMove(bool isWhite, bool isKingSide) {
        bits::U64 kingBitboard = isWhite ? _board.getWhiteKingBitboard() : _board.getBlackKingBitboard();
        int from = isWhite ? 3 : 59;
        int to = isKingSide ? (isWhite ? 2 : 58) : (isWhite ? 4 : 60);

        kingBitboard &= ~(1ULL << from);
        kingBitboard |= (1ULL << to);

        if (isWhite) {
            _board.getWhiteKingBitboard() = kingBitboard;
        } else {
            _board.getBlackKingBitboard() = kingBitboard;
        }
    }

    void MoveMaker::unmakeTemporaryKingMove(bool isWhite, bool isKingSide) {
        bits::U64 kingBitboard = isWhite ? _board.getWhiteKingBitboard() : _board.getBlackKingBitboard();
        int from = isKingSide ? (isWhite ? 2 : 58) : (isWhite ? 4 : 60);
        int to = isWhite ? 3 : 59;

        kingBitboard &= ~(1ULL << from);
        kingBitboard |= (1ULL << to);

        if (isWhite) {
            _board.getWhiteKingBitboard() = kingBitboard;
        } else {
            _board.getBlackKingBitboard() = kingBitboard;
        }
    }

    void MoveMaker::unmakeMove(Move move, bool wasWhite) {
        // If the move is a castle, update the bitboards and return
        if (move.isAnyCastle()) {
            unmakeCastleMove(wasWhite, move.isKingCastle());

            // Update white or black bitmasks
            if (wasWhite) {
                if (move.isKingCastle()) {
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 1);
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 2);
                    _board.getWhitePiecesBitmask() |= (1ULL << 3);
                    _board.getWhitePiecesBitmask() |= (1ULL << 0);
                } else {
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 5);
                    _board.getWhitePiecesBitmask() &= ~(1ULL << 4);
                    _board.getWhitePiecesBitmask() |= (1ULL << 3);
                    _board.getWhitePiecesBitmask() |= (1ULL << 7);
                }
            } else {
                if (move.isKingCastle()) {
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 57);
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 58);
                    _board.getBlackPiecesBitmask() |= (1ULL << 59);
                    _board.getBlackPiecesBitmask() |= (1ULL << 56);
                } else {
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 61);
                    _board.getBlackPiecesBitmask() &= ~(1ULL << 60);
                    _board.getBlackPiecesBitmask() |= (1ULL << 59);
                    _board.getBlackPiecesBitmask() |= (1ULL << 63);
                }
            }

            _board.fillOccupiedPiecesBitmask();
            _board.fillEmptySquaresBitmask();

            return;
        }

        // Get the from and to indices
        int from = move.getBitIndexFrom();
        int to = move.getBitIndexTo();
        
        // Piece type of piece being moved
        PieceType movedPieceType;

        // If the move was a promotion, set the moved piece to a pawn of the same color
        // Else, set the moved piece to the piece occupying the to square
        if (move.isAnyPromo()) {
            movedPieceType = wasWhite ? PieceType::W_PAWN : PieceType::B_PAWN;
        } else {
            movedPieceType = _board.getPieceTypeAtIndex(to);
        }

        // Place back the moved piece
        _board.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) |= (1ULL << from);
        _board.setPieceTypeAtIndex(from, movedPieceType);

        if (wasWhite) {
            _board.getWhitePiecesBitmask() |= (1ULL << from);
        } else {
            _board.getBlackPiecesBitmask() |= (1ULL << from);
        }

        // If the move was not a promotion, remove the piece in the bitboard
        // Else, remove the bit for the promoted piece
        if (not move.isAnyPromo()) {
            _board.getBitboardFromIndex(pieceTypeToInt(movedPieceType)) &= ~(1ULL << to);
        } else {
            PieceType promotionPieceType = getPromotionPieceType(move.getFlag(), wasWhite);
            _board.getBitboardFromIndex(pieceTypeToInt(promotionPieceType)) &= ~(1ULL << to);
        }

        if (wasWhite) {
            _board.getWhitePiecesBitmask() &= ~(1ULL << to);
        } else {
            _board.getBlackPiecesBitmask() &= ~(1ULL << to);
        }

        // If the move was a capture, place back the captured piece
        // else set the square to empty
        if (move.isAnyCapture()) {
            int captureIndex = move.isEpCapture() ? (wasWhite ? to - 8 : to + 8) : to;
            _board.getBitboardFromIndex(pieceTypeToInt(_board.getLastCapturedPiece())) |= (1ULL << captureIndex);
            _board.setPieceTypeAtIndex(captureIndex, _board.getLastCapturedPiece());

            if (move.isEpCapture()) {
                _board.setPieceTypeAtIndex(to, PieceType::EMPTY);
            }

            if (wasWhite) {
                _board.getBlackPiecesBitmask() |= (1ULL << captureIndex);
            } else {
                _board.getWhitePiecesBitmask() |= (1ULL << captureIndex);
            }

        } else {
            _board.setPieceTypeAtIndex(to, PieceType::EMPTY);
        }

        if (move.isDoublePawnPush()) {
            _board.setEnPessantTarget(0ULL);
        }

        _board.fillOccupiedPiecesBitmask();
        _board.fillEmptySquaresBitmask();
    }
}