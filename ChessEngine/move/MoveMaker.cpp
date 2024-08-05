#include "MoveMaker.h"

namespace move {

namespace {

board::PieceType getPromotionPieceType(
    const int promotionFlag, 
    const bool isWhite) 
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
    board::BitBoards& bitBoards, 
    board::GameStateBitMasks& gameStateBitMasks, 
    board::SquaresLookup& squaresLookup, 
    perft::SearchMemory& searchMemory,
    board::ZHasher& zHasher
) : _bitboards(bitBoards), 
    _gameStateBitMasks(gameStateBitMasks), 
    _squaresLookup(squaresLookup), 
    _searchMemory(searchMemory), 
    _zHasher(zHasher)
{}

void MoveMaker::makeMove(
    const Move move, 
    const bool isWhite, 
    const int currentDepth) 
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

    handleEnPessantMemory(move, currentDepth, isWhite, toIndex);
    handleNoCaptureCount(move, movedPieceType, currentDepth);

    _gameStateBitMasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeCastleMove(
    const bool isWhite,
    const bool isKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = isKingSide ? 1 : 5;
        fromRookInd = isKingSide ? 0 : 7;
        toRookInd = isKingSide ? 2 : 4;
        
        _bitboards.clearWhiteKingBit(fromKingInd);
        _bitboards.setWhiteKingBit(toKingInd);
        _bitboards.clearWhiteRooksBit(fromRookInd);
        _bitboards.setWhiteRooksBit(toRookInd);

        _gameStateBitMasks.clearWhitePiecesBit(fromKingInd);
        _gameStateBitMasks.setWhitePiecesBit(toKingInd);
        _gameStateBitMasks.clearWhitePiecesBit(fromRookInd);
        _gameStateBitMasks.setWhitePiecesBit(toRookInd);

        _squaresLookup.setPieceTypeAtIndex(fromKingInd, board::PieceType::EMPTY);
        _squaresLookup.setPieceTypeAtIndex(toKingInd, board::PieceType::W_KING);
        _squaresLookup.setPieceTypeAtIndex(fromRookInd, board::PieceType::EMPTY);
        _squaresLookup.setPieceTypeAtIndex(toRookInd, board::PieceType::W_ROOK);
    } else {
        fromKingInd = 59;
        toKingInd = isKingSide ? 57 : 61;
        fromRookInd = isKingSide ? 56 : 63;
        toRookInd = isKingSide ? 58 : 60;

        _bitboards.clearBlackKingBit(fromKingInd);
        _bitboards.setBlackKingBit(toKingInd);
        _bitboards.clearBlackRooksBit(fromRookInd);
        _bitboards.setBlackRooksBit(toRookInd);

        _gameStateBitMasks.clearBlackPiecesBit(fromKingInd);
        _gameStateBitMasks.setBlackPiecesBit(toKingInd);
        _gameStateBitMasks.clearBlackPiecesBit(fromRookInd);
        _gameStateBitMasks.setBlackPiecesBit(toRookInd);

        _squaresLookup.setPieceTypeAtIndex(fromKingInd, board::PieceType::EMPTY);
        _squaresLookup.setPieceTypeAtIndex(toKingInd, board::PieceType::B_KING);
        _squaresLookup.setPieceTypeAtIndex(fromRookInd, board::PieceType::EMPTY);
        _squaresLookup.setPieceTypeAtIndex(toRookInd, board::PieceType::B_ROOK);
    }

    _gameStateBitMasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::unmakeCastleMove(
    const bool isWhite,
    const bool wasKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = wasKingSide ? 1 : 5;
        fromRookInd = wasKingSide ? 0 : 7;
        toRookInd = wasKingSide ? 2 : 4;
        
        _bitboards.clearWhiteKingBit(toKingInd);
        _bitboards.setWhiteKingBit(fromKingInd);
        _bitboards.clearWhiteRooksBit(toRookInd);
        _bitboards.setWhiteRooksBit(fromRookInd);

        _gameStateBitMasks.setWhitePiecesBit(fromKingInd);
        _gameStateBitMasks.clearWhitePiecesBit(toKingInd);
        _gameStateBitMasks.setWhitePiecesBit(fromRookInd);
        _gameStateBitMasks.clearWhitePiecesBit(toRookInd);

        _squaresLookup.setPieceTypeAtIndex(fromKingInd, board::PieceType::W_KING);
        _squaresLookup.setPieceTypeAtIndex(toKingInd, board::PieceType::EMPTY);
        _squaresLookup.setPieceTypeAtIndex(fromRookInd, board::PieceType::W_ROOK);
        _squaresLookup.setPieceTypeAtIndex(toRookInd, board::PieceType::EMPTY);
    } else {
        fromKingInd = 59;
        toKingInd = wasKingSide ? 57 : 61;
        fromRookInd = wasKingSide ? 56 : 63;
        toRookInd = wasKingSide ? 58 : 60;

        _bitboards.setBlackKingBit(fromKingInd);
        _bitboards.clearBlackKingBit(toKingInd);
        _bitboards.setBlackRooksBit(fromRookInd);
        _bitboards.clearBlackRooksBit(toRookInd);

        _gameStateBitMasks.setBlackPiecesBit(fromKingInd);
        _gameStateBitMasks.clearBlackPiecesBit(toKingInd);
        _gameStateBitMasks.setBlackPiecesBit(fromRookInd);
        _gameStateBitMasks.clearBlackPiecesBit(toRookInd);

        _squaresLookup.setPieceTypeAtIndex(toKingInd, board::PieceType::EMPTY);
        _squaresLookup.setPieceTypeAtIndex(fromKingInd, board::PieceType::B_KING);
        _squaresLookup.setPieceTypeAtIndex(toRookInd, board::PieceType::EMPTY);
        _squaresLookup.setPieceTypeAtIndex(fromRookInd, board::PieceType::B_ROOK);
    }

    _gameStateBitMasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeTemporaryKingMove(
    const bool isWhite, 
    const bool isKingSide) 
{
    int from = isWhite ? 3 : 59;

    int to = isKingSide ? (isWhite ? 2 : 58) 
                        : (isWhite ? 4 : 60);

    if (isWhite) {
        _bitboards.clearWhiteKingBit(from);
        _bitboards.setWhiteKingBit(to);
    } else {
        _bitboards.clearBlackKingBit(from);
        _bitboards.setBlackKingBit(to);
    }
}

void MoveMaker::unmakeTemporaryKingMove(
    const bool isWhite, 
    const bool isKingSide) 
{
    int from = isKingSide ? (isWhite ? 2 : 58) 
                          : (isWhite ? 4 : 60);

    int to = isWhite ? 3 : 59;

    if (isWhite) {
        _bitboards.clearWhiteKingBit(from);
        _bitboards.setWhiteKingBit(to);
    } else {
        _bitboards.clearBlackKingBit(from);
        _bitboards.setBlackKingBit(to);
    }
}

board::PieceType MoveMaker::pickUpPiece(
    const bool isWhite, 
    const int fromIndex) 
{
    board::PieceType movedPieceType = _squaresLookup.getPieceTypeAtIndex(fromIndex);
    assert(movedPieceType != board::PieceType::EMPTY);
    
    _squaresLookup.setPieceTypeAtIndex(fromIndex, board::PieceType::EMPTY);

    _bitboards.clearPieceTypeBit(fromIndex, movedPieceType);

    if (isWhite) {
        _gameStateBitMasks.clearWhitePiecesBit(fromIndex);
    } else {
        _gameStateBitMasks.clearBlackPiecesBit(fromIndex);
    }

    return movedPieceType;
}

void MoveMaker::putDownPiece(
    const move::Move &move, 
    const bool isWhite, 
    const int toIndex, 
    const board::PieceType movedPieceType) 
{
    if (move.isAnyPromo()) {
        board::PieceType promotionPieceType = ::move::getPromotionPieceType(move.getFlag(), isWhite);
        _bitboards.setPieceTypeBit(toIndex, promotionPieceType);
        _squaresLookup.setPieceTypeAtIndex(toIndex, promotionPieceType);
    } else {
        _bitboards.setPieceTypeBit(toIndex, movedPieceType);
        _squaresLookup.setPieceTypeAtIndex(toIndex, movedPieceType);
    }

    if (isWhite) {
        _gameStateBitMasks.setWhitePiecesBit(toIndex);
    } else {
        _gameStateBitMasks.setBlackPiecesBit(toIndex);
    }
}

void MoveMaker::handleNoCaptureCount(
    const move::Move &move, 
    const board::PieceType movedPieceType, 
    const int currentDepth) 
{
    if (not move.isAnyCapture() && (movedPieceType != board::PieceType::W_PAWN && movedPieceType != board::PieceType::B_PAWN)) {
        _searchMemory.incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }
    else {
        _searchMemory.resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }
}

void MoveMaker::handleEnPessantMemory(
    const move::Move &move, 
    const int currentDepth, 
    const bool isWhite, 
    const int toIndex) 
{
    if (move.isDoublePawnPush()) {
        _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, isWhite ? (1ULL << (toIndex - 8)) : (1ULL << (toIndex + 8)));
    }
    else {
        _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
    }
}


void MoveMaker::handleCapture(
    const move::Move &move, 
    const bool isWhite, 
    const int toIndex, 
    const int currentDepth) 
{
    if (move.isAnyCapture()) {
        // Calculate index of captured piece
        int captureIndex = move.isEpCapture() ? (isWhite ? toIndex - 8 : toIndex + 8) : toIndex;
        board::PieceType capturedPieceType = _squaresLookup.getPieceTypeAtIndex(captureIndex);
        assert(capturedPieceType != board::PieceType::EMPTY);
        
        _searchMemory.setLastCapturedPieceAtDepth(currentDepth, capturedPieceType);

        // Remove captured piece from board representations
        _bitboards.clearPieceTypeBit(captureIndex, capturedPieceType);

        if (isWhite) {
            _gameStateBitMasks.clearBlackPiecesBit(captureIndex);
        } else {
            _gameStateBitMasks.clearWhitePiecesBit(captureIndex);
        }

        if (move.isEpCapture()) {
            _squaresLookup.setPieceTypeAtIndex(captureIndex, board::PieceType::EMPTY);
        }
    }
}


void MoveMaker::unmakeMove(
    const Move move, 
    const bool wasWhite, 
    const int currentDepth) 
{
    // If the move is a castle, update the bitboards and return
    if (move.isAnyCastle()) {
        unmakeCastleMove(wasWhite, move.isKingCastle());

        return;
    }

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

    _gameStateBitMasks.updOccupiedAndEmptySquaresBitmasks();
}
void MoveMaker::handleUncapturing(
    const move::Move &move, 
    const bool wasWhite, 
    const int toIndex, 
    const int currentDepth)
{
    // If the move was a capture, place back the captured piece
    // else set the square to empty
    if (move.isAnyCapture()) {
        int captureIndex = move.isEpCapture() ? (wasWhite ? toIndex - 8 : toIndex + 8) : toIndex;
        board::PieceType capturedPieceType = _searchMemory.getLastCapturedPieceAtDepth(currentDepth);

        _bitboards.setPieceTypeBit(captureIndex, capturedPieceType);
        _squaresLookup.setPieceTypeAtIndex(captureIndex, capturedPieceType);

        if (move.isEpCapture()) {
            _squaresLookup.setPieceTypeAtIndex(toIndex, board::PieceType::EMPTY);
        }

        if (wasWhite) {
            _gameStateBitMasks.setBlackPiecesBit(captureIndex);
        } else {
            _gameStateBitMasks.setWhitePiecesBit(captureIndex);
        }
    } else {
        _squaresLookup.setPieceTypeAtIndex(toIndex, board::PieceType::EMPTY);
    }
}

void MoveMaker::putBackMovedPiece(
    const int fromIndex, 
    const board::PieceType movedPieceType, 
    const bool wasWhite, 
    const move::Move &move, 
    const int toIndex) 
{
    _bitboards.setPieceTypeBit(fromIndex, movedPieceType);
    _squaresLookup.setPieceTypeAtIndex(fromIndex, movedPieceType);

    if (wasWhite) {
        _gameStateBitMasks.setWhitePiecesBit(fromIndex);
    } else {
        _gameStateBitMasks.setBlackPiecesBit(fromIndex);
    }

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.isAnyPromo()) {
        _bitboards.clearPieceTypeBit(toIndex, movedPieceType);
    } else {
        board::PieceType promotionPieceType = ::move::getPromotionPieceType(move.getFlag(), wasWhite);
        _bitboards.clearPieceTypeBit(toIndex, promotionPieceType);
    }

    if (wasWhite) {
        _gameStateBitMasks.clearWhitePiecesBit(toIndex);
    } else {
        _gameStateBitMasks.clearBlackPiecesBit(toIndex);
    }
}

board::PieceType MoveMaker::determineMovedPieceType(
    const Move move, 
    const int toIndex,
     const bool wasWhite) 
{
    // Piece type of piece being moved
    board::PieceType movedPieceType;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    bool moveIsAnyPromo = move.isAnyPromo();
    
    if (moveIsAnyPromo) {
        movedPieceType = wasWhite ? board::PieceType::W_PAWN : board::PieceType::B_PAWN;
    } else {
        movedPieceType = _squaresLookup.getPieceTypeAtIndex(toIndex);
    }

    return movedPieceType;
}

} // namespace move