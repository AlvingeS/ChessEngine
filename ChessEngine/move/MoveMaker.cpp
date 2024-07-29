#include "MoveMaker.h"

namespace move {

    MoveMaker::MoveMaker(
        BitBoardUpdater& bitBoardUpdater,
        BitMaskUpdater& bitMaskUpdater,
        LookupUpdater& lookupUpdater,
        perft::SearchMemory& searchMemory,
        board::ZHasher& zHasher
    ) : _bitBoardUpdater(bitBoardUpdater), 
        _bitMaskUpdater(bitMaskUpdater), 
        _lookupUpdater(lookupUpdater), 
        _searchMemory(searchMemory), 
        _zHasher(zHasher)
    {}

    void MoveMaker::makeMove(Move move, bool isWhite, int currentDepth) {
        // If the move is a castle, update the bitboards and return
        bool retFlag;
        tryCastling(move, isWhite, retFlag);
        if (retFlag)
            return;

        // Get the from and to indices
        int fromIndex = move.getBitIndexFrom();
        int toIndex = move.getBitIndexTo();
        
        board::PieceType movedPieceType = pickUpPiece(move, isWhite, fromIndex);

        // If the move is a capture, update the last captured piece and its bitboard
        handleCapture(move, isWhite, toIndex, currentDepth);
        putDownPiece(move, isWhite, toIndex, movedPieceType);

        handleEnPessantMemory(move, currentDepth, isWhite, toIndex);
        handleNoCaptureCount(move, movedPieceType, currentDepth);

        _bitMaskUpdater.updateOccupiedAndEmptyBitmasks();
    }

    void MoveMaker::tryCastling(move::Move &move, bool isWhite, bool &retFlag) {
        retFlag = true;
        if (move.isAnyCastle()) {
            _bitBoardUpdater.makeCastleMove(isWhite, move.isKingCastle());
            _bitMaskUpdater.makeCastleMove(isWhite, move.isKingCastle());
            _lookupUpdater.makeCastleMove(isWhite, move.isKingCastle());

            return;
        }
        retFlag = false;
    }
    
    void MoveMaker::handleNoCaptureCount(move::Move &move, board::PieceType movedPieceType, int currentDepth) {
        if (not move.isAnyCapture() && (movedPieceType != board::PieceType::W_PAWN && movedPieceType != board::PieceType::B_PAWN)) {
            _searchMemory.incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        }
        else {
            _searchMemory.resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        }
    }

    void MoveMaker::handleEnPessantMemory(move::Move &move, int currentDepth, bool isWhite, int toIndex) {
        if (move.isDoublePawnPush()) {
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, isWhite ? (1ULL << (toIndex - 8)) : (1ULL << (toIndex + 8)));
        }
        else {
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        }
    }

    board::PieceType MoveMaker::pickUpPiece(move::Move &move, bool isWhite, int fromIndex) {
        board::PieceType movedPieceType = _lookupUpdater.getPieceTypeAtIndex(fromIndex);
        _lookupUpdater.setPieceTypeAtIndex(fromIndex, board::PieceType::EMPTY);

        _bitBoardUpdater.removeBitAtIndexForPieceType(fromIndex, movedPieceType);

        if (isWhite) {
            _bitMaskUpdater.clearBitForWhitePiecesAtIndex(fromIndex);
        } else {
            _bitMaskUpdater.clearBitForBlackPiecesAtIndex(fromIndex);
        }

        return movedPieceType;
    }

    void MoveMaker::putDownPiece(move::Move &move, bool isWhite, int toIndex, board::PieceType movedPieceType) {
        if (move.isAnyPromo()) {
            board::PieceType promotionPieceType = _lookupUpdater.getPromotionPieceType(move.getFlag(), isWhite);
            _bitBoardUpdater.setBitAtIndexForPieceType(toIndex, promotionPieceType);
            _lookupUpdater.setPieceTypeAtIndex(toIndex, promotionPieceType);
        } else {
            _bitBoardUpdater.setBitAtIndexForPieceType(toIndex, movedPieceType);
            _lookupUpdater.setPieceTypeAtIndex(toIndex, movedPieceType);
        }

        if (isWhite) {
            _bitMaskUpdater.setBitForWhitePiecesAtIndex(toIndex);
        } else {
            _bitMaskUpdater.setBitForBlackPiecesAtIndex(toIndex);
        }
    }

    void MoveMaker::handleCapture(move::Move &move, bool isWhite, int toIndex, int currentDepth) {
        if (move.isAnyCapture()) {
            // Calculate index of captured piece
            int captureIndex = move.isEpCapture() ? (isWhite ? toIndex - 8 : toIndex + 8) : toIndex;
            board::PieceType capturedPieceType = _lookupUpdater.getPieceTypeAtIndex(captureIndex);
            _searchMemory.setLastCapturedPieceAtDepth(currentDepth, capturedPieceType);

            // Remove captured piece from board representations
            _bitBoardUpdater.removeBitAtIndexForPieceType(captureIndex, capturedPieceType);

            if (isWhite) {
                _bitMaskUpdater.clearBitForBlackPiecesAtIndex(captureIndex);
            }
            else {
                _bitMaskUpdater.clearBitForWhitePiecesAtIndex(captureIndex);
            }

            if (move.isEpCapture()) {
                _lookupUpdater.removeCapturedPieceFromLookup(captureIndex);
            }
        }
    }


    void MoveMaker::unmakeMove(Move move, bool wasWhite, int currentDepth) {
        // If the move is a castle, update the bitboards and return
        bool retFlag;
        tryUncastling(move, wasWhite, retFlag);
        if (retFlag)
            return;

        // Get the from and to indices
        int fromIndex = move.getBitIndexFrom();
        int toIndex = move.getBitIndexTo();
        
        board::PieceType movedPieceType = determineMovedPieceType(move, toIndex, wasWhite);

        // Place back the moved piece
        putBackMovedPiece(fromIndex, movedPieceType, wasWhite, move, toIndex);
        handleUncapturing(move, wasWhite, toIndex, currentDepth);

        if (move.isDoublePawnPush()) {
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        }

        if (not move.isAnyCapture() && (movedPieceType != board::PieceType::W_PAWN && movedPieceType != board::PieceType::B_PAWN)) {
            _searchMemory.decrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        }

        _bitMaskUpdater.updateOccupiedAndEmptyBitmasks();
    }
    void MoveMaker::handleUncapturing(move::Move &move, bool wasWhite, int toIndex, int currentDepth)
    {
        // If the move was a capture, place back the captured piece
        // else set the square to empty
        if (move.isAnyCapture())
        {
            int captureIndex = move.isEpCapture() ? (wasWhite ? toIndex - 8 : toIndex + 8) : toIndex;
            board::PieceType capturedPieceType = _searchMemory.getLastCapturedPieceAtDepth(currentDepth);

            _bitBoardUpdater.setBitAtIndexForPieceType(captureIndex, capturedPieceType);
            _lookupUpdater.setPieceTypeAtIndex(captureIndex, capturedPieceType);

            if (move.isEpCapture()) {
                _lookupUpdater.setPieceTypeAtIndex(toIndex, board::PieceType::EMPTY);
            }

            if (wasWhite) {
                _bitMaskUpdater.setBitForBlackPiecesAtIndex(captureIndex);
            }
            else {
                _bitMaskUpdater.setBitForWhitePiecesAtIndex(captureIndex);
            }
        } else {
            _lookupUpdater.setPieceTypeAtIndex(toIndex, board::PieceType::EMPTY);
        }
    }
    void MoveMaker::putBackMovedPiece(int fromIndex, board::PieceType movedPieceType, bool wasWhite, move::Move &move, int toIndex) {
        _bitBoardUpdater.setBitAtIndexForPieceType(fromIndex, movedPieceType);
        _lookupUpdater.setPieceTypeAtIndex(fromIndex, movedPieceType);

        if (wasWhite) {
            _bitMaskUpdater.setBitForWhitePiecesAtIndex(fromIndex);
        } else {
            _bitMaskUpdater.setBitForBlackPiecesAtIndex(fromIndex);
        }

        // If the move was not a promotion, remove the piece in the bitboard
        // Else, remove the bit for the promoted piece
        if (not move.isAnyPromo()) {
            _bitBoardUpdater.removeBitAtIndexForPieceType(toIndex, movedPieceType);
        } else {
            board::PieceType promotionPieceType = _lookupUpdater.getPromotionPieceType(move.getFlag(), wasWhite);
            _bitBoardUpdater.removeBitAtIndexForPieceType(toIndex, promotionPieceType);
        }

        if (wasWhite) {
            _bitMaskUpdater.clearBitForWhitePiecesAtIndex(toIndex);
        } else {
            _bitMaskUpdater.clearBitForBlackPiecesAtIndex(toIndex);
        }
    }
    void MoveMaker::tryUncastling(move::Move &move, bool wasWhite, bool &retFlag) {
        retFlag = true;
        if (move.isAnyCastle())
        {
            _bitBoardUpdater.unmakeCastleMove(wasWhite, move.isKingCastle());
            _bitMaskUpdater.unmakeCastleMove(wasWhite, move.isKingCastle());
            _lookupUpdater.unmakeCastleMove(wasWhite, move.isKingCastle());

            return;
        }
        retFlag = false;
    }

    board::PieceType MoveMaker::determineMovedPieceType(Move move, int toIndex, bool wasWhite) {
        // Piece type of piece being moved
        board::PieceType movedPieceType;

        // If the move was a promotion, set the moved piece to a pawn of the same color
        // Else, set the moved piece to the piece occupying the to square
        bool moveIsAnyPromo = move.isAnyPromo();
        
        if (moveIsAnyPromo) {
            movedPieceType = wasWhite ? board::PieceType::W_PAWN : board::PieceType::B_PAWN;
        } else {
            movedPieceType = _lookupUpdater.getPieceTypeAtIndex(toIndex);
        }

        return movedPieceType;
    }
}