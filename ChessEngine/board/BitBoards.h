#pragma once

#include <cassert>

#include "ChessEngine/common.h"
#include "ChessEngine/board/PieceType.h"

namespace board {

class BitBoards {

public:

    // ** Constructor **
    BitBoards();

    inline const U64& getBitboardFromPieceType(const PieceType pieceType) const 
    {
        return *(_bitboards[pieceTypeToInt(pieceType)]);
    }

    inline const U64& getBitboardFromIndex(const int index) const 
    {
        return *(_bitboards[index]);
    }

    // inline const std::vector<U64*>& getBitboards() const 
    // {
    //     return _bitboards;
    // }

    inline bool pieceTypeBitIsSet(const int index, const PieceType pieceType) const 
    {
        assert(pieceType != PieceType::EMPTY);
        return (*(_bitboards[pieceTypeToInt(pieceType)]) & (1ULL << index)) != 0;
    }

    inline void clearPieceTypeBit(const int index, const PieceType pieceType) 
    {
        assert(pieceType != PieceType::EMPTY);
        assert(pieceTypeBitIsSet(index, pieceType));
        *(_bitboards[pieceTypeToInt(pieceType)]) &= ~(1ULL << index);
    }

    inline void setPieceTypeBit(const int index, const PieceType pieceType) 
    {
        assert(pieceType != PieceType::EMPTY);
        assert(!pieceTypeBitIsSet(index, pieceType));
        *(_bitboards[pieceTypeToInt(pieceType)]) |= (1ULL << index);
    }

    #define DEFINE_BITBOARD_MACROS(PIECE_NAME, VARIABLE_NAME) \
        inline void set##PIECE_NAME##Bit(const int square) \
        { \
            VARIABLE_NAME |= (1ULL << square); \
        } \
        inline void clear##PIECE_NAME##Bit(const int square) \
        { \
            VARIABLE_NAME &= ~(1ULL << square); \
        } \
        inline const U64& get##PIECE_NAME##Bitboard() const \
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
std::vector<U64*> _bitboards;

// White bitboards
U64 _whitePawnsBitboard;
U64 _whiteKnightsBitboard;
U64 _whiteBishopsBitboard;
U64 _whiteRooksBitboard;
U64 _whiteQueensBitboard;
U64 _whiteKingBitboard;

// Black bitboards
U64 _blackPawnsBitboard;
U64 _blackKnightsBitboard;
U64 _blackBishopsBitboard;
U64 _blackRooksBitboard;
U64 _blackQueensBitboard;
U64 _blackKingBitboard;

};

} // namespace board