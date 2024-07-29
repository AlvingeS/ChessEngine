#include "BitBoardUpdater.h"

namespace move {

BitBoardUpdater::BitBoardUpdater(board::BitBoards& bitboards) 
    : _bitboards(bitboards) 
{}

void BitBoardUpdater::makeCastleMove(
    const bool isWhite, 
    const bool isKingSide)
{
    if (isWhite) {
        if (isKingSide) {
            _bitboards.getWhiteKingBitboard() &= ~(1ULL << 3);
            _bitboards.getWhiteKingBitboard() |= (1ULL << 1);
            _bitboards.getWhiteRooksBitboard() &= ~(1ULL);
            _bitboards.getWhiteRooksBitboard() |= (1ULL << 2);
        } else {
            _bitboards.getWhiteKingBitboard() &= ~(1ULL << 3);
            _bitboards.getWhiteKingBitboard() |= (1ULL << 5);
            _bitboards.getWhiteRooksBitboard() &= ~(1ULL << 7);
            _bitboards.getWhiteRooksBitboard() |= (1ULL << 4);
        }
    } else {
        if (isKingSide) {
            _bitboards.getBlackKingBitboard() &= ~(1ULL << 59);
            _bitboards.getBlackKingBitboard() |= (1ULL << 57);
            _bitboards.getBlackRooksBitboard() &= ~(1ULL << 56);
            _bitboards.getBlackRooksBitboard() |= (1ULL << 58);
        } else {
            _bitboards.getBlackKingBitboard() &= ~(1ULL << 59);
            _bitboards.getBlackKingBitboard() |= (1ULL << 61);
            _bitboards.getBlackRooksBitboard() &= ~(1ULL << 63);
            _bitboards.getBlackRooksBitboard() |= (1ULL << 60);
        }
    }
}

void BitBoardUpdater::unmakeCastleMove(
    const bool wasWhite, 
    const bool wasKingSide) 
{
    if (wasWhite) {
        if (wasKingSide) {
            _bitboards.getWhiteKingBitboard() &= ~(1ULL << 1);
            _bitboards.getWhiteKingBitboard() |= (1ULL << 3);
            _bitboards.getWhiteRooksBitboard() &= ~(1ULL << 2);
            _bitboards.getWhiteRooksBitboard() |= (1ULL);
        } else {
            _bitboards.getWhiteKingBitboard() &= ~(1ULL << 5);
            _bitboards.getWhiteKingBitboard() |= (1ULL << 3);
            _bitboards.getWhiteRooksBitboard() &= ~(1ULL << 4);
            _bitboards.getWhiteRooksBitboard() |= (1ULL << 7);
        }
    } else {
        if (wasKingSide) {
            _bitboards.getBlackKingBitboard() &= ~(1ULL << 57);
            _bitboards.getBlackKingBitboard() |= (1ULL << 59);
            _bitboards.getBlackRooksBitboard() &= ~(1ULL << 58);
            _bitboards.getBlackRooksBitboard() |= (1ULL << 56);
        } else {
            _bitboards.getBlackKingBitboard() &= ~(1ULL << 61);
            _bitboards.getBlackKingBitboard() |= (1ULL << 59);
            _bitboards.getBlackRooksBitboard() &= ~(1ULL << 60);
            _bitboards.getBlackRooksBitboard() |= (1ULL << 63);
        }
    }
}

void BitBoardUpdater::makeTemporaryKingMove(
    const bool isWhite, 
    const bool isKingSide) 
{
    U64 kingBitboard = isWhite ? _bitboards.getWhiteKingBitboard() 
                               : _bitboards.getBlackKingBitboard();

    int from = isWhite ? 3 : 59;

    int to = isKingSide ? (isWhite ? 2 : 58) 
                        : (isWhite ? 4 : 60);

    kingBitboard &= ~(1ULL << from);
    kingBitboard |= (1ULL << to);

    if (isWhite) {
        _bitboards.getWhiteKingBitboard() = kingBitboard;
    } else {
        _bitboards.getBlackKingBitboard() = kingBitboard;
    }
}

void BitBoardUpdater::unmakeTemporaryKingMove(
    const bool isWhite, 
    const bool isKingSide) 
{
    U64 kingBitboard = isWhite ? _bitboards.getWhiteKingBitboard() 
                               : _bitboards.getBlackKingBitboard();

    int from = isKingSide ? (isWhite ? 2 : 58) 
                          : (isWhite ? 4 : 60);

    int to = isWhite ? 3 : 59;

    kingBitboard &= ~(1ULL << from);
    kingBitboard |= (1ULL << to);

    if (isWhite) {
        _bitboards.getWhiteKingBitboard() = kingBitboard;
    } else {
        _bitboards.getBlackKingBitboard() = kingBitboard;
    }
}

} // namespace move