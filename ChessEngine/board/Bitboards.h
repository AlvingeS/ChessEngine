#pragma once

#include <cassert>

#include "ChessEngine/common.h"
#include "ChessEngine/board/PieceType.h"

namespace board {

class Bitboards {

public:

    // ** Constructor **
    Bitboards();

    inline const bitboard& getBitboardFromPieceType(PieceType pieceType) const 
    {
        return *(_bitboards[pieceTypeToInt(pieceType)]);
    }

    inline const bitboard& getBitboardFromIndex(int index) const 
    {
        return *(_bitboards[index]);
    }

    // inline const std::vector<bitboard*>& getBitboards() const 
    // {
    //     return _bitboards;
    // }

    inline bool pieceTypeBitIsSet(int index, PieceType pieceType) const 
    {
        assert(pieceType != PieceType::EMPTY);
        return (*(_bitboards[pieceTypeToInt(pieceType)]) & (1ULL << index)) != 0;
    }

    inline void clearPieceTypeBit(int index, PieceType pieceType)
    {
        assert(pieceType != PieceType::EMPTY);
        assert(pieceTypeBitIsSet(index, pieceType));
        *(_bitboards[pieceTypeToInt(pieceType)]) &= ~(1ULL << index);
    }

    inline void setPieceTypeBit(int index, PieceType pieceType) 
    {
        assert(pieceType != PieceType::EMPTY);
        assert(!pieceTypeBitIsSet(index, pieceType));
        *(_bitboards[pieceTypeToInt(pieceType)]) |= (1ULL << index);
    }

    #define DEFINE_BITBOARD_MACROS(PIECE_NAME, VARIABLE_NAME) \
        inline void set##PIECE_NAME##Bit(int square) \
        { \
            VARIABLE_NAME |= (1ULL << square); \
        } \
        inline void clear##PIECE_NAME##Bit(int square) \
        { \
            VARIABLE_NAME &= ~(1ULL << square); \
        } \
        inline const bitboard& get##PIECE_NAME##Bitboard() const \
        { \
            return VARIABLE_NAME; \
        }

    DEFINE_BITBOARD_MACROS(WhitePawns, _whitePawnsBitboard)
    DEFINE_BITBOARD_MACROS(WhiteKnights, _whiteKnightsBitboard)
    DEFINE_BITBOARD_MACROS(WhiteBishops, _whiteBishopsBitboard)
    DEFINE_BITBOARD_MACROS(WhiteRooks, _whiteRooksBitboard)
    DEFINE_BITBOARD_MACROS(WhiteQueens, _whiteQueensBitboard)
    DEFINE_BITBOARD_MACROS(WhiteKing, _whiteKingBitboard)
    DEFINE_BITBOARD_MACROS(BlackPawns, _blackPawnsBitboard)
    DEFINE_BITBOARD_MACROS(BlackKnights, _blackKnightsBitboard)
    DEFINE_BITBOARD_MACROS(BlackBishops, _blackBishopsBitboard)
    DEFINE_BITBOARD_MACROS(BlackRooks, _blackRooksBitboard)
    DEFINE_BITBOARD_MACROS(BlackQueens, _blackQueensBitboard)
    DEFINE_BITBOARD_MACROS(BlackKing, _blackKingBitboard)

    // ** Functions **
    inline void resetBitboards() 
    {
        for (int i = 0; i < 12; i++)
            *(_bitboards[i]) = 0;
    }

private:
// ** Data **
std::vector<bitboard*> _bitboards;

// White bitboards
bitboard _whitePawnsBitboard;
bitboard _whiteKnightsBitboard;
bitboard _whiteBishopsBitboard;
bitboard _whiteRooksBitboard;
bitboard _whiteQueensBitboard;
bitboard _whiteKingBitboard;

// Black bitboards
bitboard _blackPawnsBitboard;
bitboard _blackKnightsBitboard;
bitboard _blackBishopsBitboard;
bitboard _blackRooksBitboard;
bitboard _blackQueensBitboard;
bitboard _blackKingBitboard;

};

} // namespace board