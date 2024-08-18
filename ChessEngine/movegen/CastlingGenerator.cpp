#include "ChessEngine/movegen/CastlingGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {
CastlingGenerator::CastlingGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks,
    move::MoveMaker& moveMaker, 
    move::MoveRetractor& moveRetractor, 
    CheckDetection* checkDetection)
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
    , _moveMakerRef(moveMaker)
    , _moveRetractorRef(moveRetractor)
    , _checkDetection(checkDetection) 
{
    _whiteKingSideCastleBitmask = whiteKingSideCastleMask;
    _whiteQueenSideCastleBitmask = whiteQueenSideCastleMask;
    _blackKingSideCastleBitmask = blackKingSideCastleMask;
    _blackQueenSideCastleBitmask = blackQueenSideCastleMask;
}

void CastlingGenerator::generate(
    bool isWhite,
    Movelist& movelist,
    unsigned char castlingRights) 
{
    if (castlingRights == 0) {
        return;
    }
    
    if (isWhite) {
        if (castlingRights & 0b0001)
            genSingleCastleMove(isWhite, true, movelist);

        if (castlingRights & 0b0010)
            genSingleCastleMove(isWhite, false, movelist);
    } else {
        if (castlingRights & 0b0100)
            genSingleCastleMove(isWhite, true, movelist);

        if (castlingRights & 0b1000)
            genSingleCastleMove(isWhite, false, movelist);
    }
}

bool CastlingGenerator::kingAndRookOnCastlingSquares(
    bool isWhite,
    bool isKingSide) const
{
    bool kingBitEnabled;
    bool rookBitEnabled;
    
    if (isWhite) {
        kingBitEnabled = (_bitboardsRef.getWhiteKingBitboard() & (1ULL << 3)) != 0;
        
        if (!kingBitEnabled) {
            return false;
        }

        if (isKingSide) {
            rookBitEnabled = (_bitboardsRef.getWhiteRooksBitboard() & (1ULL << 0)) != 0;
            return rookBitEnabled;
        } else {
            rookBitEnabled = (_bitboardsRef.getWhiteRooksBitboard() & (1ULL << 7)) != 0;
            return rookBitEnabled;
        }
    } else {
        kingBitEnabled = (_bitboardsRef.getBlackKingBitboard() & (1ULL << 59)) != 0;
        
        if (!kingBitEnabled) {
            return false;
        }

        if (isKingSide) {
            rookBitEnabled = (_bitboardsRef.getBlackRooksBitboard() & (1ULL << 56)) != 0;
            return rookBitEnabled;
        } else {
            rookBitEnabled = (_bitboardsRef.getBlackRooksBitboard() & (1ULL << 63)) != 0;
            return rookBitEnabled;
        }
    }

    return false;
}

void CastlingGenerator::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    _moveMakerRef.makeTemporaryKingMove(isWhite, isKingSide);
}

void CastlingGenerator::unmakeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    _moveRetractorRef.unmakeTemporaryKingMove(isWhite, isKingSide);
}

void CastlingGenerator::genSingleCastleMove(
    bool isWhite,
    bool isKingSide,
    Movelist& moveListRef)
{                                                  
    // Check that there are no pieces between the king and rook
    bitmask spaceBetweenCastlersBitmask = isWhite ? (isKingSide ? _whiteKingSideCastleBitmask 
                                                            : _whiteQueenSideCastleBitmask)
                                              : (isKingSide ? _blackKingSideCastleBitmask
                                                            : _blackQueenSideCastleBitmask);
    
    if ((spaceBetweenCastlersBitmask & _gameStateBitmasksRef.getOccupiedPiecesBitmask()) != 0)
        return;

    // Check that the king and rook are on the correct squares
    if (!kingAndRookOnCastlingSquares(isWhite, isKingSide))
        return;

    // Check that we are not currently in check
    if (_checkDetection->isInCheck(isWhite))
        return;

    // Move king one square towards the rook, check that the king is not in check
    makeTemporaryKingMove(isWhite, isKingSide);
    
    if (_checkDetection->isInCheck(isWhite)) {
        unmakeTemporaryKingMove(isWhite, isKingSide);
        return;
    }
    
    unmakeTemporaryKingMove(isWhite, isKingSide);

    int moveFlag = isKingSide ? move::Move::KING_CASTLE_FLAG 
                              : move::Move::QUEEN_CASTLE_FLAG;    

    moveListRef.addMove(move::Move(0, 0, moveFlag));
}

} // namespace movegen