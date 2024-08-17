#include "MoveMaker.h"

namespace move {

namespace {

board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite) 
{
    switch(promotionFlag) {
        case Move::KNIGHT_PROMO_FLAG:
            return isWhite ? board::PieceType::W_KNIGHT : board::PieceType::B_KNIGHT;
            break;
        case Move::BISHOP_PROMO_FLAG:
            return isWhite ? board::PieceType::W_BISHOP : board::PieceType::B_BISHOP;
            break;
        case Move::ROOK_PROMO_FLAG:
            return isWhite ? board::PieceType::W_ROOK : board::PieceType::B_ROOK;
            break;
        case Move::QUEEN_PROMO_FLAG:
            return isWhite ? board::PieceType::W_QUEEN : board::PieceType::B_QUEEN;
            break;
        case Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_KNIGHT : board::PieceType::B_KNIGHT;
            break;
        case Move::BISHOP_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_BISHOP : board::PieceType::B_BISHOP;
            break;
        case Move::ROOK_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_ROOK : board::PieceType::B_ROOK;
            break;
        case Move::QUEEN_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_QUEEN : board::PieceType::B_QUEEN;
            break;
    }

    return board::PieceType::EMPTY;
}

} // namespace

MoveMaker::MoveMaker(
    board::Bitboards& bitBoards, 
    board::GameStateBitmasks& gameStateBitmasks, 
    board::SquaresLookup& squaresLookup, 
    perft::SearchMemory& searchMemory,
    board::ZHasher& zHasher
) : _bitboardsRef(bitBoards), 
    _gameStateBitmasksRef(gameStateBitmasks), 
    _squaresLookupRef(squaresLookup), 
    _searchMemoryRef(searchMemory), 
    _zHasherRef(zHasher)
{}

void MoveMaker::makeMove(
    const Move& move, 
    bool isWhite, 
    int currentDepth) 
{
    // If the move is a castle, update data and return
    if (move.isAnyCastle()) {
        makeCastleMove(isWhite, move.isKingCastle());

        return;
    }

    // Get the from and to indices
    int fromIndex = move.getBitIndexFrom();
    int toIndex = move.getBitIndexTo();
    assert(fromIndex != toIndex);

    board::PieceType movedPieceType = pickUpPiece(isWhite, fromIndex);

    // If the move is a capture, update the last captured piece and its bitboard
    handleCapture(move, isWhite, toIndex, currentDepth);
    putDownPiece(move, isWhite, toIndex, movedPieceType);

    handleEnPessantMemory(move, isWhite, currentDepth, toIndex);
    handleNoCaptureCount(move, currentDepth, movedPieceType);

    _gameStateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeCastleMove(bool isWhite,bool isKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = isKingSide ? 1 : 5;
        fromRookInd = isKingSide ? 0 : 7;
        toRookInd = isKingSide ? 2 : 4;
        
        _bitboardsRef.clearWhiteKingBit(fromKingInd);
        _bitboardsRef.setWhiteKingBit(toKingInd);
        _bitboardsRef.clearWhiteRooksBit(fromRookInd);
        _bitboardsRef.setWhiteRooksBit(toRookInd);

        _gameStateBitmasksRef.clearWhitePiecesBit(fromKingInd);
        _gameStateBitmasksRef.setWhitePiecesBit(toKingInd);
        _gameStateBitmasksRef.clearWhitePiecesBit(fromRookInd);
        _gameStateBitmasksRef.setWhitePiecesBit(toRookInd);

        _squaresLookupRef.setPieceTypeAtIndex(fromKingInd, board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(toKingInd, board::PieceType::W_KING);
        _squaresLookupRef.setPieceTypeAtIndex(fromRookInd, board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(toRookInd, board::PieceType::W_ROOK);
    } else {
        fromKingInd = 59;
        toKingInd = isKingSide ? 57 : 61;
        fromRookInd = isKingSide ? 56 : 63;
        toRookInd = isKingSide ? 58 : 60;

        _bitboardsRef.clearBlackKingBit(fromKingInd);
        _bitboardsRef.setBlackKingBit(toKingInd);
        _bitboardsRef.clearBlackRooksBit(fromRookInd);
        _bitboardsRef.setBlackRooksBit(toRookInd);

        _gameStateBitmasksRef.clearBlackPiecesBit(fromKingInd);
        _gameStateBitmasksRef.setBlackPiecesBit(toKingInd);
        _gameStateBitmasksRef.clearBlackPiecesBit(fromRookInd);
        _gameStateBitmasksRef.setBlackPiecesBit(toRookInd);

        _squaresLookupRef.setPieceTypeAtIndex(fromKingInd, board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(toKingInd, board::PieceType::B_KING);
        _squaresLookupRef.setPieceTypeAtIndex(fromRookInd, board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(toRookInd, board::PieceType::B_ROOK);
    }

    _gameStateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::unmakeCastleMove(bool isWhite,bool wasKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = wasKingSide ? 1 : 5;
        fromRookInd = wasKingSide ? 0 : 7;
        toRookInd = wasKingSide ? 2 : 4;
        
        _bitboardsRef.clearWhiteKingBit(toKingInd);
        _bitboardsRef.setWhiteKingBit(fromKingInd);
        _bitboardsRef.clearWhiteRooksBit(toRookInd);
        _bitboardsRef.setWhiteRooksBit(fromRookInd);

        _gameStateBitmasksRef.setWhitePiecesBit(fromKingInd);
        _gameStateBitmasksRef.clearWhitePiecesBit(toKingInd);
        _gameStateBitmasksRef.setWhitePiecesBit(fromRookInd);
        _gameStateBitmasksRef.clearWhitePiecesBit(toRookInd);

        _squaresLookupRef.setPieceTypeAtIndex(fromKingInd, board::PieceType::W_KING);
        _squaresLookupRef.setPieceTypeAtIndex(toKingInd, board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(fromRookInd, board::PieceType::W_ROOK);
        _squaresLookupRef.setPieceTypeAtIndex(toRookInd, board::PieceType::EMPTY);
    } else {
        fromKingInd = 59;
        toKingInd = wasKingSide ? 57 : 61;
        fromRookInd = wasKingSide ? 56 : 63;
        toRookInd = wasKingSide ? 58 : 60;

        _bitboardsRef.setBlackKingBit(fromKingInd);
        _bitboardsRef.clearBlackKingBit(toKingInd);
        _bitboardsRef.setBlackRooksBit(fromRookInd);
        _bitboardsRef.clearBlackRooksBit(toRookInd);

        _gameStateBitmasksRef.setBlackPiecesBit(fromKingInd);
        _gameStateBitmasksRef.clearBlackPiecesBit(toKingInd);
        _gameStateBitmasksRef.setBlackPiecesBit(fromRookInd);
        _gameStateBitmasksRef.clearBlackPiecesBit(toRookInd);

        _squaresLookupRef.setPieceTypeAtIndex(toKingInd, board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(fromKingInd, board::PieceType::B_KING);
        _squaresLookupRef.setPieceTypeAtIndex(toRookInd, board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(fromRookInd, board::PieceType::B_ROOK);
    }

    _gameStateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    int from = isWhite ? 3 : 59;

    int to = isKingSide ? (isWhite ? 2 : 58) 
                        : (isWhite ? 4 : 60);

    if (isWhite) {
        _bitboardsRef.clearWhiteKingBit(from);
        _bitboardsRef.setWhiteKingBit(to);
    } else {
        _bitboardsRef.clearBlackKingBit(from);
        _bitboardsRef.setBlackKingBit(to);
    }
}

void MoveMaker::unmakeTemporaryKingMove(bool isWhite, bool isKingSide) 
{
    int from = isKingSide ? (isWhite ? 2 : 58) 
                          : (isWhite ? 4 : 60);

    int to = isWhite ? 3 : 59;

    if (isWhite) {
        _bitboardsRef.clearWhiteKingBit(from);
        _bitboardsRef.setWhiteKingBit(to);
    } else {
        _bitboardsRef.clearBlackKingBit(from);
        _bitboardsRef.setBlackKingBit(to);
    }
}

board::PieceType MoveMaker::pickUpPiece(bool isWhite, int fromIndex) 
{
    board::PieceType movedPieceType = _squaresLookupRef.getPieceTypeAtIndex(fromIndex);
    assert(movedPieceType != board::PieceType::EMPTY);
    
    _squaresLookupRef.setPieceTypeAtIndex(fromIndex, board::PieceType::EMPTY);

    _bitboardsRef.clearPieceTypeBit(fromIndex, movedPieceType);

    if (isWhite) {
        _gameStateBitmasksRef.clearWhitePiecesBit(fromIndex);
    } else {
        _gameStateBitmasksRef.clearBlackPiecesBit(fromIndex);
    }

    return movedPieceType;
}

void MoveMaker::putDownPiece(
    const move::Move& move, 
    bool isWhite, 
    int toIndex, 
    board::PieceType movedPieceType) 
{
    if (move.isAnyPromo()) {
        board::PieceType promotionPieceType = ::move::getPromotionPieceType(move.getFlag(), isWhite);
        _bitboardsRef.setPieceTypeBit(toIndex, promotionPieceType);
        _squaresLookupRef.setPieceTypeAtIndex(toIndex, promotionPieceType);
    } else {
        _bitboardsRef.setPieceTypeBit(toIndex, movedPieceType);
        _squaresLookupRef.setPieceTypeAtIndex(toIndex, movedPieceType);
    }

    if (isWhite) {
        _gameStateBitmasksRef.setWhitePiecesBit(toIndex);
    } else {
        _gameStateBitmasksRef.setBlackPiecesBit(toIndex);
    }
}

void MoveMaker::handleNoCaptureCount(
    const move::Move& move, 
    int currentDepth, 
    board::PieceType movedPieceType)
{
    if (not move.isAnyCapture() && (movedPieceType != board::PieceType::W_PAWN && movedPieceType != board::PieceType::B_PAWN)) {
        _searchMemoryRef.incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }
    else {
        _searchMemoryRef.resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }
}

void MoveMaker::handleEnPessantMemory(
    const move::Move& move, 
    bool isWhite, 
    int currentDepth, 
    int toIndex) 
{
    if (move.isDoublePawnPush()) {
        _searchMemoryRef.setEnPessantTargetAtDepth(currentDepth + 1, isWhite ? (1ULL << (toIndex - 8)) : (1ULL << (toIndex + 8)));
    }
    else {
        _searchMemoryRef.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
    }
}


void MoveMaker::handleCapture(
    const move::Move& move, 
    bool isWhite, 
    int toIndex, 
    int currentDepth) 
{
    if (move.isAnyCapture()) {
        // Calculate index of captured piece
        int captureIndex = move.isEpCapture() ? (isWhite ? toIndex - 8 : toIndex + 8) : toIndex;
        board::PieceType capturedPieceType = _squaresLookupRef.getPieceTypeAtIndex(captureIndex);
        assert(capturedPieceType != board::PieceType::EMPTY);
        
        _searchMemoryRef.setLastCapturedPieceAtDepth(currentDepth, capturedPieceType);

        // Remove captured piece from board representations
        _bitboardsRef.clearPieceTypeBit(captureIndex, capturedPieceType);

        if (isWhite) {
            _gameStateBitmasksRef.clearBlackPiecesBit(captureIndex);
        } else {
            _gameStateBitmasksRef.clearWhitePiecesBit(captureIndex);
        }

        if (move.isEpCapture()) {
            _squaresLookupRef.setPieceTypeAtIndex(captureIndex, board::PieceType::EMPTY);
        }
    }
}


void MoveMaker::unmakeMove(
    const Move& move, 
    bool wasWhite, 
    int currentDepth) 
{
    // If the move is a castle, update the bitboards and return
    if (move.isAnyCastle()) {
        unmakeCastleMove(wasWhite, move.isKingCastle());

        return;
    }

    // Get the from and to indices
    int fromIndex = move.getBitIndexFrom();
    int toIndex = move.getBitIndexTo();
    
    board::PieceType movedPieceType = determineMovedPieceType(move, wasWhite, toIndex);

    // Place back the moved piece
    putBackMovedPiece(move, wasWhite, fromIndex, toIndex, movedPieceType);
    handleUncapturing(move, wasWhite, toIndex, currentDepth);

    if (move.isDoublePawnPush()) {
        _searchMemoryRef.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
    }

    if (not move.isAnyCapture() && (movedPieceType != board::PieceType::W_PAWN && movedPieceType != board::PieceType::B_PAWN)) {
        _searchMemoryRef.decrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }

    _gameStateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}
void MoveMaker::handleUncapturing(
    const move::Move& move, 
    bool wasWhite, 
    int toIndex, 
    int currentDepth)
{
    // If the move was a capture, place back the captured piece
    // else set the square to empty
    if (move.isAnyCapture()) {
        int captureIndex = move.isEpCapture() ? (wasWhite ? toIndex - 8 : toIndex + 8) : toIndex;
        board::PieceType capturedPieceType = _searchMemoryRef.getLastCapturedPieceAtDepth(currentDepth);

        _bitboardsRef.setPieceTypeBit(captureIndex, capturedPieceType);
        _squaresLookupRef.setPieceTypeAtIndex(captureIndex, capturedPieceType);

        if (move.isEpCapture()) {
            _squaresLookupRef.setPieceTypeAtIndex(toIndex, board::PieceType::EMPTY);
        }

        if (wasWhite) {
            _gameStateBitmasksRef.setBlackPiecesBit(captureIndex);
        } else {
            _gameStateBitmasksRef.setWhitePiecesBit(captureIndex);
        }
    } else {
        _squaresLookupRef.setPieceTypeAtIndex(toIndex, board::PieceType::EMPTY);
    }
}

void MoveMaker::putBackMovedPiece(
    const move::Move& move, 
    bool wasWhite, 
    int fromIndex, 
    int toIndex,
    board::PieceType movedPieceType)
{
    _bitboardsRef.setPieceTypeBit(fromIndex, movedPieceType);
    _squaresLookupRef.setPieceTypeAtIndex(fromIndex, movedPieceType);

    if (wasWhite) {
        _gameStateBitmasksRef.setWhitePiecesBit(fromIndex);
    } else {
        _gameStateBitmasksRef.setBlackPiecesBit(fromIndex);
    }

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.isAnyPromo()) {
        _bitboardsRef.clearPieceTypeBit(toIndex, movedPieceType);
    } else {
        board::PieceType promotionPieceType = ::move::getPromotionPieceType(move.getFlag(), wasWhite);
        _bitboardsRef.clearPieceTypeBit(toIndex, promotionPieceType);
    }

    if (wasWhite) {
        _gameStateBitmasksRef.clearWhitePiecesBit(toIndex);
    } else {
        _gameStateBitmasksRef.clearBlackPiecesBit(toIndex);
    }
}

board::PieceType MoveMaker::determineMovedPieceType(
    const Move& move, 
    bool wasWhite,
    int toIndex) const
{
    // Piece type of piece being moved
    board::PieceType movedPieceType;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    bool moveIsAnyPromo = move.isAnyPromo();
    
    if (moveIsAnyPromo) {
        movedPieceType = wasWhite ? board::PieceType::W_PAWN : board::PieceType::B_PAWN;
    } else {
        movedPieceType = _squaresLookupRef.getPieceTypeAtIndex(toIndex);
    }

    return movedPieceType;
}

} // namespace move