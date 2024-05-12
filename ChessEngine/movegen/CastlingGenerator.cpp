#include "CastlingGenerator.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"
#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/MoveMaker.h"

namespace movegen {
    CastlingGenerator::CastlingGenerator(game::ChessBoard& board, game::MoveMaker& moveMaker, int& moveIndex, CommonLogic* commonLogic, CheckDetection* checkDetection) 
        : _board(board),
          _moveMaker(moveMaker),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic),
          _checkDetection(checkDetection) {
        _whiteKingSideCastleBitmask = bits::whiteKingSideCastleMask;
        _whiteQueenSideCastleBitmask = bits::whiteQueenSideCastleMask;
        _blackKingSideCastleBitmask = bits::blackKingSideCastleMask;
        _blackQueenSideCastleBitmask = bits::blackQueenSideCastleMask;
    }

    void CastlingGenerator::generate(bool isWhite, std::vector<game::Move>& moveList, unsigned char castlingRights) {
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

    bool CastlingGenerator::kingAndRookOnCastlingSquares(bool isWhite, bool isKingSide) {
        if (isWhite) {
            bool kingBitEnabled = (_board.getWhiteKingBitboard() & (1ULL << 3)) != 0;
            
            if (isKingSide) {
                bool rookBitEnabled = (_board.getWhiteRooksBitboard() & (1ULL << 0)) != 0;
                return kingBitEnabled && rookBitEnabled;
            } else {
                bool rookBitEnabled = (_board.getWhiteRooksBitboard() & (1ULL << 7)) != 0;
                return kingBitEnabled && rookBitEnabled;
            }
        } else {
            bool kingBitEnabled = (_board.getBlackKingBitboard() & (1ULL << 59)) != 0;
            
            if (isKingSide) {
                bool rookBitEnabled = (_board.getBlackRooksBitboard() & (1ULL << 56)) != 0;
                return kingBitEnabled && rookBitEnabled;
            } else {
                bool rookBitEnabled = (_board.getBlackRooksBitboard() & (1ULL << 63)) != 0;
                return kingBitEnabled && rookBitEnabled;
            }
        }

        return false;
    }

    void CastlingGenerator::makeTemporaryKingMove(bool isWhite, bool isKingSide) {
        _moveMaker.makeTemporaryKingMove(isWhite, isKingSide);
    }

    void CastlingGenerator::unmakeTemporaryKingMove(bool isWhite, bool isKingSide) {
        _moveMaker.unmakeTemporaryKingMove(isWhite, isKingSide);
    }

    void CastlingGenerator::genSingleCastleMove(bool isWhite, bool isKingSide, std::vector<game::Move>& moveList) {                                                  
        // Check that there are no pieces between the king and rook
        bits::U64 spaceBetweenCastlersBitmask = isWhite ? (isKingSide ? _whiteKingSideCastleBitmask : _whiteQueenSideCastleBitmask)
                                                        : (isKingSide ? _blackKingSideCastleBitmask : _blackQueenSideCastleBitmask);
        
        if ((spaceBetweenCastlersBitmask & _board.getOccupiedPiecesBitmask()) != 0) {
            return;
        }

        // Check that the king and rook are on the correct squares
        if (!kingAndRookOnCastlingSquares(isWhite, isKingSide)) {
            return;
        }

        // Check that we are not currently in check
        if (_checkDetection->isInCheck(isWhite)) {
            return;
        }

        // Move king one square towards the rook, check that the king is not in check
        makeTemporaryKingMove(isWhite, isKingSide);
        
        if (_checkDetection->isInCheck(isWhite)) {
            unmakeTemporaryKingMove(isWhite, isKingSide);
            return;
        }
        
        unmakeTemporaryKingMove(isWhite, isKingSide);

        int moveFlag = isKingSide ? game::Move::KING_CASTLE_FLAG : game::Move::QUEEN_CASTLE_FLAG;        
        _commonLogic->addMove(0, 0, moveFlag, moveList, _moveIndex);
    }
}