#include "CastlingGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {
CastlingGenerator::CastlingGenerator(
    board::BitBoards& bitboards,
    board::GameStateBitMasks& gameStateBitmasks,
    move::MoveMaker& moveMaker, int& moveIndex,
    CommonLogic* commonLogic,
    CheckDetection* checkDetection)
    : _bitboards(bitboards)
    , _gameStateBitmasks(gameStateBitmasks)
    , _moveMaker(moveMaker)
    , _moveIndex(moveIndex)
    , _commonLogic(commonLogic)
    , _checkDetection(checkDetection) 
{
    _whiteKingSideCastleBitmask = whiteKingSideCastleMask;
    _whiteQueenSideCastleBitmask = whiteQueenSideCastleMask;
    _blackKingSideCastleBitmask = blackKingSideCastleMask;
    _blackQueenSideCastleBitmask = blackQueenSideCastleMask;
}

void CastlingGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList,
    unsigned char castlingRights) 
{
    if (castlingRights == 0) {
        return;
    }
    
    if (isWhite) {
        if (castlingRights & 0b0001)
            genSingleCastleMove(isWhite, true, moveList);

        if (castlingRights & 0b0010)
            genSingleCastleMove(isWhite, false, moveList);
    } else {
        if (castlingRights & 0b0100)
            genSingleCastleMove(isWhite, true, moveList);

        if (castlingRights & 0b1000)
            genSingleCastleMove(isWhite, false, moveList);
    }
}

bool CastlingGenerator::kingAndRookOnCastlingSquares(
    bool isWhite,
    bool isKingSide)
{
    bool kingBitEnabled;
    bool rookBitEnabled;
    
    if (isWhite) {
        kingBitEnabled = (_bitboards.getWhiteKingBitboard() & (1ULL << 3)) != 0;
        
        if (!kingBitEnabled) {
            return false;
        }

        if (isKingSide) {
            rookBitEnabled = (_bitboards.getWhiteRooksBitboard() & (1ULL << 0)) != 0;
            return rookBitEnabled;
        } else {
            rookBitEnabled = (_bitboards.getWhiteRooksBitboard() & (1ULL << 7)) != 0;
            return rookBitEnabled;
        }
    } else {
        kingBitEnabled = (_bitboards.getBlackKingBitboard() & (1ULL << 59)) != 0;
        
        if (!kingBitEnabled) {
            return false;
        }

        if (isKingSide) {
            rookBitEnabled = (_bitboards.getBlackRooksBitboard() & (1ULL << 56)) != 0;
            return rookBitEnabled;
        } else {
            rookBitEnabled = (_bitboards.getBlackRooksBitboard() & (1ULL << 63)) != 0;
            return rookBitEnabled;
        }
    }

    return false;
}

void CastlingGenerator::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    _moveMaker.makeTemporaryKingMove(isWhite, isKingSide);
}

void CastlingGenerator::unmakeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    _moveMaker.unmakeTemporaryKingMove(isWhite, isKingSide);
}

void CastlingGenerator::genSingleCastleMove(
    bool isWhite,
    bool isKingSide,
    std::vector<move::Move>& moveList)
{                                                  
    // Check that there are no pieces between the king and rook
    U64 spaceBetweenCastlersBitmask = isWhite ? (isKingSide ? _whiteKingSideCastleBitmask 
                                                            : _whiteQueenSideCastleBitmask)
                                              : (isKingSide ? _blackKingSideCastleBitmask
                                                            : _blackQueenSideCastleBitmask);
    
    if ((spaceBetweenCastlersBitmask & _gameStateBitmasks.getOccupiedPiecesBitmask()) != 0)
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

    _commonLogic->addMove(0, 0, moveFlag, moveList, _moveIndex);
}

} // namespace movegen